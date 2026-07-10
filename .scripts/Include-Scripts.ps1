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
