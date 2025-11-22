$app_output_path = Join-Path $PSScriptRoot -ChildPath ".." "Output"

$latest_version_slug = "[//]: # (APP_LATESTVERSION)"
$help_output_slug = "[//]: # (APP_HELPOUTPUT)"
$command_output_slug_prefix = "[//]: # (CMD_HELPOUTPUT"

$allfiles = Get-ChildItem -Path $app_output_path -File -Filter "*.exe" -Recurse
Write-Host $allfiles.Length
Write-Host $allfiles

# Build Apps List
$apps = @{}
foreach ($file in $allfiles | Where-Object { $_.FullName.contains("Release") }) {
    $apps[$file.Name] = $file
}
foreach ($file in $allfiles | Where-Object { $_.FullName.contains("Debug") }) {
    if ( $apps.ContainsKey($file.Name) ) {
        continue
    }
    $apps[$file.Name] = $file
}


# Process each App and generate Version and About help text
Write-Host "Generating $($apps.Count) applications Help Text"
foreach ( $app in $apps.GetEnumerator() ) {
    $appName = $app.Key
    $appFile = $app.Value
    $appBaseName = [System.IO.Path]::GetFileNameWithoutExtension($appName)

    Write-Host "Processing App: ${appName}" -ForegroundColor Cyan

    $docs_folder = Join-Path $PSScriptRoot -ChildPath ".." ".docs"
    $docs_helptext_folder = Join-Path $docs_folder -ChildPath "HelpText"

    # TODO: Adjust paths in output content
    # TODO: Support multi-command apps

    Write-Host "  Generating Version info for: $appName"
    $version_output_file = Join-Path -Path $docs_helptext_folder -ChildPath ($appBaseName + ".Version.txt")
    $version_text = (& $appFile.FullName -!)
    Write-Host "    Writing Version info to: $version_output_file"
    Set-Content -Path $version_output_file -Value $version_text -Encoding UTF8

    Write-Host "  Generating About info for: $appName"
    $about_output_file = Join-Path -Path $docs_helptext_folder -ChildPath ($appBaseName + ".About.txt")
    $about_text = (& $appFile.FullName -?)
    Write-Host "    Writing About info to: $about_output_file"
    Set-Content -Path $about_output_file -Value $about_text -Encoding UTF8
}


# Process each App and bind help text into docs
Write-Host "Binding $($apps.Count) applications Docs"
foreach ( $app in $apps.GetEnumerator() ) {
    $appName = $app.Key
    $appFile = $app.Value
    $appBaseName = [System.IO.Path]::GetFileNameWithoutExtension($appName)

    Write-Host "Processing App: ${appName}" -ForegroundColor Cyan

    $docs_folder = Join-Path $PSScriptRoot -ChildPath ".." ".docs"
    $docs_helptext_folder = Join-Path $docs_folder -ChildPath "HelpText"

    $doc_file = Join-Path -Path $docs_folder -ChildPath ($appBaseName + ".md")
    if (!(Test-Path -Path $doc_file -PathType Leaf)) {
        Write-Host "  WARNING: Doc file not found: $doc_file" -ForegroundColor Yellow
        continue
    }

    $doc_content = Get-Content -Path $doc_file
    $new_content = @()

    $replacing_version = $false
    $replacing_about = $false
    foreach($line in $doc_content) {
        
        if ($line -eq "``````") {
            if ($replacing_version -or $replacing_about) {
                $replacing_version = $false
                $replacing_about = $false
                continue
            }
        }

        if ($replacing_version -or $replacing_about) {
            continue
        }

        if ($line -eq $latest_version_slug) {
            $version_file = Join-Path -Path $docs_helptext_folder -ChildPath ($appBaseName + ".Version.txt")
            if (Test-Path -Path $version_file -PathType Leaf) {
                $replacing_version = $true
                $version_text = Get-Content -Path $version_file
                Write-Host "  Replacing Version slug in doc: $doc_file"
                $new_content += $line
                $new_content += ""
                $new_content += "> ${appBaseName} -!"
                $new_content += ""
                $new_content += "``````text"
                $new_content += $version_text
                $new_content += "``````"
            }
            else {
                Write-Host "  WARNING: Version file not found: $version_file" -ForegroundColor Yellow
            }
            continue
        }

        if ($line -eq $help_output_slug) {
            $about_file = Join-Path -Path $docs_helptext_folder -ChildPath ($appBaseName + ".About.txt")
            if (Test-Path -Path $about_file -PathType Leaf) {
                $replacing_about = $true
                $about_text = Get-Content -Path $about_file
                Write-Host "  Replacing About slug in doc: $doc_file"
                $new_content += $line
                $new_content += ""
                $new_content += "> ${appBaseName} -?"
                $new_content += ""
                $new_content += "``````text"
                $new_content += $about_text
                $new_content += "``````"
            }
            else {
                Write-Host "  WARNING: About file not found: $about_file" -ForegroundColor Yellow
            }
            continue
        }

        $new_content += $line
    }

    Set-Content -Path $doc_file -Value $new_content -Encoding UTF8
}

Write-Host "DONE: Help Text Population Complete." -ForegroundColor Green
