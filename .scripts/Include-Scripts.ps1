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

    $start_info = New-Object System.Diagnostics.ProcessStartInfo
    $start_info.FileName               = $app_full_path
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

    # Capture the output and error streams
    $stdout = $process.StandardOutput.ReadToEnd()
    $stderr = $process.StandardError.ReadToEnd()

    # Wait for the process to exit
    $process.WaitForExit()

    return @{ StdOut = $stdout; StdErr = $stderr; ExitCode = $process.ExitCode }
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
