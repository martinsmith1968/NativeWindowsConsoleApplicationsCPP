#---------------------------------------------------------------------------------------------------
function Build-AppsList {
    param (
        [string]$base_path_name,
        [bool]$showAppsList = $true
    )

    $allfiles = @{}
    if (Test-Path -Path $base_path_name -PathType Container) {
        $allfiles = Get-ChildItem -Path $base_path_name -File -Filter "*.exe" -Recurse
        Write-Host "Found $($allfiles.Length) candidate executables"
    }

    if ($allfiles.Length -gt 0) {
        $allfiles = $allfiles | Where-Object { -Not $_.FullName.contains("deps") } | Where-Object { -Not $_.FullName.contains("_build-") }
    }

    $apps = @{}

    foreach ($file in $allfiles | Where-Object { $_.FullName.contains("Release", 'InvariantCultureIgnoreCase') }) {
        $file_name_only = [System.IO.Path]::GetFileNameWithoutExtension($file.Name)
        $apps[$file_name_only] = $file
    }

    foreach ($file in $allfiles | Where-Object { $_.FullName.contains("Debug", 'InvariantCultureIgnoreCase') }) {
        $file_name_only = [System.IO.Path]::GetFileNameWithoutExtension($file.Name)
        if ( $apps.ContainsKey($file_name_only) ) {
            continue
        }
        $apps[$file_name_only] = $file
    }

    Write-Host "$($apps.Count) Apps found" -ForegroundColor Green
    if ($showAppsList) {
        foreach( $app in $apps.GetEnumerator() ) {
            Write-Host "$($app.Key) : $($app.Value.FullName)" -ForegroundColor DarkGray
        }
    }

    return $apps
}

#---------------------------------------------------------------------------------------------------
function Search-AppByName {
    param (
        [hashtable]$apps,
        [string]$app_name,
        [bool]$showFoundDetails = $true
    )

    $app = $null
    Write-Host "Looking for app : $($app_name)..." -ForegroundColor Yellow
    if ( $apps.ContainsKey($app_name) ) {
        $app = $apps[$app_name]
    }

    if ($showFoundDetails) {
        if ($null -ne $app) {
            Write-Host "Found app : $($app_name) : $($app.FullName)" -ForegroundColor Green
        } else {
            Write-Host "App not found : $($app_name)" -ForegroundColor Red
        }
    }

    return $app
}

#---------------------------------------------------------------------------------------------------
function Copy-AppByName-ToTarget {
    param (
        [hashtable]$apps,
        [string]$app_name,
        [string]$target_folder
    )

    $app = Search-AppByName $apps $app_name
    if ($null -eq $app) {
        return $false
    }

    Write-Host "Copying app : $($app_name)..." -ForegroundColor Yellow
    Copy-Item -Path $app.FullName -Destination $target_folder -Force
    return $true
}

#---------------------------------------------------------------------------------------------------
function Invoke-CaptureOutput {
    param (
        [string]$app_full_path,
        [string]$arguments
    )

    # Single-byte encoding, so the app's output bytes round-trip exactly
    $raw_encoding = [System.Text.Encoding]::Latin1

    $start_info = New-Object System.Diagnostics.ProcessStartInfo
    $start_info.FileName = $app_full_path
    # See Invoke-AppRawOutput for why WorkingDirectory is set explicitly rather than relying on
    # Environment.CurrentDirectory.
    $start_info.WorkingDirectory = (Get-Location).Path
    if ($arguments.contains("|")) {
        $arguments.Split("|") | ForEach-Object { $start_info.ArgumentList.Add($_) }
    } else {
        $start_info.Arguments          = $arguments
    }
    $start_info.RedirectStandardOutput = $true
    $start_info.RedirectStandardError  = $true
    $start_info.UseShellExecute        = $false
    $start_info.CreateNoWindow         = $true

    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $start_info
    $process.Start() | Out-Null

    # Read the raw pipe bytes via BaseStream rather than through StandardOutput/StandardError's
    # text-reading methods: see Invoke-AppRawOutput for why (BOM auto-detection, stream-drain deadlock).
    $stdout_buffer = New-Object System.IO.MemoryStream
    $stderr_buffer = New-Object System.IO.MemoryStream
    $stdout_task = $process.StandardOutput.BaseStream.CopyToAsync($stdout_buffer)
    $stderr_task = $process.StandardError.BaseStream.CopyToAsync($stderr_buffer)
    [System.Threading.Tasks.Task]::WaitAll(@($stdout_task, $stderr_task))
    $process.WaitForExit()

    $stdout = $raw_encoding.GetString($stdout_buffer.ToArray())
    $stderr = $raw_encoding.GetString($stderr_buffer.ToArray())

    return @{ StdOut = $stdout; StdErr = $stderr; ExitCode = $process.ExitCode }
}

#---------------------------------------------------------------------------------------------------
function Invoke-AppRawOutput {
    param (
        [string]$app_full_path,
        [string[]]$arguments = @()
    )

    # Single-byte encoding, so the app's output bytes round-trip exactly (as the tests compare raw bytes)
    $raw_encoding = [System.Text.Encoding]::Latin1

    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $app_full_path
    # Explicitly pass PowerShell's logical location : plain Process.Start falls back to
    # Environment.CurrentDirectory when WorkingDirectory is unset, which Push-Location/Set-Location
    # don't reliably keep in sync with $PWD (PowerShell's own "&" invocation passes $PWD explicitly
    # for this reason).
    $psi.WorkingDirectory = (Get-Location).Path
    $arguments | ForEach-Object { $psi.ArgumentList.Add($_) }
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError  = $true
    $psi.UseShellExecute        = $false
    $psi.CreateNoWindow         = $true
    $proc = [System.Diagnostics.Process]::Start($psi)

    # Read the raw pipe bytes via BaseStream rather than through StandardOutput/StandardError's
    # text-reading methods: StreamReader always sniffs the first bytes for a BOM regardless of
    # any encoding assigned to ProcessStartInfo, which would silently corrupt output that happens
    # to start with a byte sequence matching a UTF-8/UTF-16/UTF-32 BOM. Draining both streams
    # concurrently also avoids the classic deadlock where the child blocks writing to a full
    # stderr pipe while we're blocked reading stdout (or vice versa).
    $stdout_buffer = New-Object System.IO.MemoryStream
    $stderr_buffer = New-Object System.IO.MemoryStream
    $stdout_task = $proc.StandardOutput.BaseStream.CopyToAsync($stdout_buffer)
    $stderr_task = $proc.StandardError.BaseStream.CopyToAsync($stderr_buffer)
    [System.Threading.Tasks.Task]::WaitAll(@($stdout_task, $stderr_task))
    $proc.WaitForExit()

    return $raw_encoding.GetString($stdout_buffer.ToArray())
}

#---------------------------------------------------------------------------------------------------
function Remove-Item-IfExists {
    param (
        [string]$filePath
    )

    if (Test-Path $filePath -PathType Leaf) {
        Remove-Item $filePath
    }
}

#---------------------------------------------------------------------------------------------------
function New-TempFileName {
    param (
        [string]$filePrefix = "Temp",
        [string]$fileExtension = "tmp",
        [bool]$createFile = $True
    )

    $value = ([guid]::NewGuid()).Guid.ToString().Replace("-", "")

    $fileName = Join-Path ( [System.IO.Path]::GetTempPath() ) "$($filePrefix)$($value).$($fileExtension)"

    if ($createFile) {
        Remove-Item-IfExists $fileName
        New-Item $fileName -ItemType File | Out-Null
    }

    return $fileName
}

#---------------------------------------------------------------------------------------------------
function Get-AppVersion {
    param (
        [string]$app_full_path
    )

    # Output of "-!" is either "<version>" or "<appname> <version>" - the version is always the last word
    $output = (Invoke-CaptureOutput -app_full_path $app_full_path -arguments "-!").StdOut.Trim()
    return ($output -split "\s+")[-1]
}
