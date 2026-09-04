function Build-AppsList {
    param (
        [string]$base_path_name,
        [bool]$showAppsist = $true
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
    if ($showAppsist) {
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
