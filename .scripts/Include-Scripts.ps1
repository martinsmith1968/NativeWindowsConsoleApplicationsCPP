function Build-AppsList {
    param (
        [string]$base_path_name
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

    foreach ($file in $allfiles | Where-Object { $_.FullName.contains("Release") }) {
        $file_name_only = [System.IO.Path]::GetFileNameWithoutExtension($file.Name)
        $apps[$file_name_only] = $file
    }

    foreach ($file in $allfiles | Where-Object { $_.FullName.contains("Debug") }) {
        $file_name_only = [System.IO.Path]::GetFileNameWithoutExtension($file.Name)
        if ( $apps.ContainsKey($file_name_only) ) {
            continue
        }
        $apps[$file_name_only] = $file
    }

    return $apps
}

#---------------------------------------------------------------------------------------------------

function Search-AppByName {
    param (
        [hashtable]$apps,
        [string]$app_name
    )
    Write-Host "Looking for app : $($app_name)..." -ForegroundColor Yellow
    if ( $apps.ContainsKey($app_name) ) {
        return $apps[$app_name]
    }
    return $null
}

#---------------------------------------------------------------------------------------------------

function Copy-AppByName-ToTarget {
    param (
        [hashtable]$apps,
        [string]$app_name,
        [string]$target_folder
    )

    $app = Search-AppByName $apps $app_name
    if ($app -eq $null) {
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
    $start_info.FileName = $app_full_path
    $start_info.Arguments = $arguments
    $start_info.RedirectStandardOutput = $true
    $start_info.RedirectStandardError = $true
    $start_info.UseShellExecute = $false
    $start_info.CreateNoWindow = $true

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
