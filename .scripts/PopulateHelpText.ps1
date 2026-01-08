$latest_version_slug        = "[//]: # (APP_LATESTVERSION)"
$help_output_slug           = "[//]: # (APP_HELPOUTPUT)"
$command_output_slug_prefix = "[//]: # (CMD_HELPOUTPUT"
$command_output_slug_suffix = ")"

$app_output_path = Join-Path $PSScriptRoot -ChildPath ".." "Output"
$temp_base_path  = [System.IO.Path]::GetTempPath()
$run_id          = [System.Guid]::NewGuid().ToString("N")
$temp_run_path   = Join-Path -Path $temp_base_path -ChildPath $run_id

Write-Host "App Output Path : $($app_output_path)"
Write-Host "Temp Path       : $($temp_base_path)"
Write-Host "Run Path        : $($temp_run_path)"


# Start

$allfiles = Get-ChildItem -Path $app_output_path -File -Filter "*.exe" -Recurse
Write-Host "Found $($allfiles.Length) candidate executables"

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

# Copy Apps to Run Folder
$run_apps = @{}
Write-Host "Copying $($apps.Count) applications to temp run folder: $temp_run_path"
New-Item -Path $temp_run_path -ItemType Directory -Force | Out-Null
foreach ( $app in $apps.GetEnumerator() ) {
    $appName = $app.Key
    $appFile = $app.Value
    $destFile = Join-Path -Path $temp_run_path -ChildPath $appName

    Write-Host "  Copying App: ${appName} to ${destFile}"
    Copy-Item -Path $appFile.FullName -Destination $destFile -Force

    $run_apps[$appName] = Get-Item -Path $destFile
}

# Process each App and generate Version and About help text
Write-Host "Generating $($run_apps.Count) applications Help Text"
foreach ( $app in $run_apps.GetEnumerator() ) {
    $appName = $app.Key
    $appFile = $app.Value
    $appBaseName = [System.IO.Path]::GetFileNameWithoutExtension($appName)

    Write-Host "Processing App: ${appName}" -ForegroundColor Cyan
    Set-Location -Path $temp_base_path

    $docs_folder = Join-Path $PSScriptRoot -ChildPath ".." ".docs"
    $docs_helptext_folder = Join-Path $docs_folder -ChildPath "HelpText"

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

    if ($about_text.IndexOf("Commands:") -ge 0) {
        # Multi-command app, extract commands
        $commands = @()
        $in_commands_section = $false
        $commands_section_blank_line_count = 0
        foreach ($line in $about_text) {
            if ($line.Trim() -eq "Commands:") {
                $in_commands_section = $true
                continue
            }
            if ($in_commands_section) {
                if ($line.Trim() -eq "") {
                    $commands_section_blank_line_count += 1
                    if ($commands_section_blank_line_count -ge 2) {
                        break
                    } else {
                        continue
                    }
                }
                $command_name = $line.Trim().Split(" ")[0].ToString()
                $commands += $command_name
            }
        }

        foreach ($cmd in $commands) {
            Write-Host "    Generating Help info for Command: $cmd"
            $cmd_help_output_file = Join-Path -Path $docs_helptext_folder -ChildPath ($appBaseName + "-" + $cmd + ".About.txt")
            $cmd_help_text = (& $appFile.FullName $cmd -?)
            Write-Host "      Writing Command Help info to: $cmd_help_output_file"
            Set-Content -Path $cmd_help_output_file -Value $cmd_help_text -Encoding UTF8
        }
    }
}


# Process each App and bind help text into docs
Write-Host "Binding $($run_apps.Count) applications Docs"
foreach ( $app in $run_apps.GetEnumerator() ) {
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
    $replacing_cmd_about = $false
    foreach($line in $doc_content) {

        if ($line -eq "``````") {
            if ($replacing_version -or $replacing_about -or $replacing_cmd_about) {
                $replacing_version = $false
                $replacing_about = $false
                $replacing_cmd_about = $false
                continue
            }
        }

        if ($replacing_version -or $replacing_about -or $replacing_cmd_about) {
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

        if ($line.StartsWith($command_output_slug_prefix) -and $line.EndsWith($command_output_slug_suffix)) {
            $command_name = $line.Substring($command_output_slug_prefix.Length, $line.Length - $command_output_slug_prefix.Length - $command_output_slug_suffix.Length).Trim()
            $commmand_help_file = Join-Path -Path $docs_helptext_folder -ChildPath ($appBaseName + "-" + $command_name + ".About.txt")
            if (Test-Path -Path $commmand_help_file -PathType Leaf) {
                $replacing_cmd_about = $true
                $cmd_help_text = Get-Content -Path $commmand_help_file
                Write-Host "  Replacing Command Help slug in doc: $doc_file for command: $command_name"
                $new_content += $line
                $new_content += ""
                $new_content += "> ${appBaseName} ${command_name} -?"
                $new_content += ""
                $new_content += "``````text"
                $new_content += $cmd_help_text
                $new_content += "``````"
            }
            else {
                Write-Host "  WARNING: Command Help file not found: $commmand_help_file" -ForegroundColor Yellow
            }
            continue
        }

        $new_content += $line
    }

    Set-Content -Path $doc_file -Value $new_content -Encoding UTF8
}

Write-Host "DONE: Help Text Population Complete." -ForegroundColor Green
