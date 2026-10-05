. $PSScriptRoot\Include-Scripts.ps1

#---------------------------------------------------------------------------------------------------
$ErrorActionPreference = "Stop"

#---------------------------------------------------------------------------------------------------
function Set-ExpectedOutput {
    param (
        [string]$app_full_path,
        [string]$arguments,
        [string]$output_filename,
        [string]$expected_output_path = $null,
        [bool]$show_output = $true
    )

    $app_name = [System.IO.Path]::GetFileNameWithoutExtension($app_full_path)

    # Single-byte encoding, so the app's output bytes round-trip exactly (as the tests compare raw bytes)
    $raw_encoding = [System.Text.Encoding]::Latin1

    if ( [string]::IsNullOrEmpty($expected_output_path) ) {
        $expected_output_path = [System.IO.Path]::GetFullPath((Join-Path -Path $PSScriptRoot -ChildPath ".." "tests" "apps" ($app_name + ".Tests") "ExpectedOutput"))
    }
    if ( -not (Test-Path -Path $expected_output_path -PathType Container) ) {
        New-Item -Path $expected_output_path -ItemType Directory -Force | Out-Null
    }

    $parameters = $arguments.Split("|")

    $example_filename = "$($output_filename).example"
    $example_full_path = Join-Path -Path $expected_output_path -ChildPath $example_filename

    Write-Host "  $($app_name) - Generating : $($example_filename)"

    # Mirror the tests (TestRunRelocatingController): run a copy of the app from a folder under TEMP,
    # with TEMP as the current directory
    $temp_path       = $env:TEMP.TrimEnd('\')
    $run_folder_path = Join-Path -Path $temp_path -ChildPath ("testgen" + [System.Guid]::NewGuid().ToString("N"))
    New-Item -Path $run_folder_path -ItemType Directory -Force | Out-Null

    try {
        $relocated_app_path = Join-Path -Path $run_folder_path -ChildPath ([System.IO.Path]::GetFileName($app_full_path))
        Copy-Item -Path $app_full_path -Destination $relocated_app_path -Force

        # Use ProcessStartInfo with redirected stdout so the binary has no Win32 console,
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = $relocated_app_path
        $psi.WorkingDirectory = $temp_path
        $parameters | ForEach-Object { $psi.ArgumentList.Add($_) }
        $psi.RedirectStandardOutput = $true
        $psi.RedirectStandardError  = $true
        $psi.StandardOutputEncoding = $raw_encoding
        $psi.UseShellExecute        = $false
        $psi.CreateNoWindow         = $true
        $proc = [System.Diagnostics.Process]::Start($psi)
        $stdout = $proc.StandardOutput.ReadToEnd()
        $proc.WaitForExit()
    }
    finally {
        Remove-Item -Path $run_folder_path -Recurse -Force -ErrorAction SilentlyContinue
    }

    Write-Host "  $($app_name) - Adjusting : $($example_filename)"
    # Placeholders are expanded by the tests (TestHelper::GetExpectedOutput) from environment variables.
    # Order matters : the run folder is under TEMP, which is under LOCALAPPDATA
    $text = $stdout
    $text = $text -replace [regex]::Escape($run_folder_path), "%RUN_FOLDERNAME%"
    $text = $text -replace [regex]::Escape($temp_path), "%TEMP%"
    $text = $text -replace [regex]::Escape($env:LOCALAPPDATA.TrimEnd([char]92)), "%LOCALAPPDATA%"
    $text = $text -replace [regex]::Escape($current_app_version), "%APP_VERSION%"
    $text = $text -replace "-${current_year}", '-%DATE_CURRENTYEAR%'
    [System.IO.File]::WriteAllText($example_full_path, $text, $raw_encoding)

    if ($show_output) {
        Write-Host "  $($app_name) - Output : $($example_filename)" -ForegroundColor Gray
        Write-Host "  $($text)" -ForegroundColor DarkGray
    }
}

#---------------------------------------------------------------------------------------------------
function Clear-ExpectedOutput {
    param (
        [string]$app_full_name
    )

    $app_name = [System.IO.Path]::GetFileNameWithoutExtension($app_full_name)

    $expected_output_path = [System.IO.Path]::GetFullPath((Join-Path -Path $PSScriptRoot -ChildPath ".." $app_name "tests" "Expectedoutput"))

    if (Test-Path -Path $expected_output_path) {
        Remove-Item -Path $expected_output_path -Include *.example -Recurse -Force
    }
}

#---------------------------------------------------------------------------------------------------

$app_output_path     = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot -ChildPath ".." "Output"))
$temp_base_path      = [System.IO.Path]::GetTempPath()
$run_id              = [System.Guid]::NewGuid().ToString("N")
$temp_run_path       = Join-Path -Path $temp_base_path -ChildPath $run_id
$current_app_version = "0.1.0-dev"
$current_year        = (Get-Date).Year

Write-Host "App Output Path : $($app_output_path)"
Write-Host "Temp Path       : $($temp_base_path)"
Write-Host "Run Path        : $($temp_run_path)"
Write-Host "App Version     : $($current_app_version)"

$env:COLUMNS = "500"


# Build Apps List
$apps = Build-AppsList -base_path_name $app_output_path
if ( $apps.Count -eq 0 ) {
    Write-Host "No apps found in output path. Please build the apps before running this script." -ForegroundColor Red
    Write-Host "Searched Path : $($app_output_path)" -ForegroundColor Red
    Write-Host "Run: cargo build --release" -ForegroundColor Yellow
    exit 1
}

#---------------------------------------------------------------------------------------------------

# Generate For : BannerText
$app_name = "BannerText"
$app = Search-AppByName -apps $apps -app_name $app_name
if ( $null -ne $app ) {
    $current_app_version = (Invoke-CaptureOutput -app_full_path $app.FullName -arguments "-!").StdOut.Trim()

    Clear-ExpectedOutput -app_full_name $app.FullName
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-?"                             -output_filename "Execute_with_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "--help"                         -output_filename "Execute_with_full_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "bob"                            -output_filename "Execute_with_text_only_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "bob|-minl|80"                   -output_filename "Execute_with_text_and_min_length_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "a|bb|ccc|dddd|eeeee"            -output_filename "Execute_with_multiple_text_lines_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "a|bb|ccc|dddd|eeeee|-ta|Center" -output_filename "Execute_with_multiple_text_lines_aligned_center_produces_expected_output"
}

# Generate For : FigLetText
$app_name = "FigLetText"
$app = Search-AppByName -apps $apps -app_name $app_name
if ( $null -ne $app ) {
    $current_app_version = (Invoke-CaptureOutput -app_full_path $app.FullName -arguments "-!").StdOut.Trim()

    Clear-ExpectedOutput -app_full_name $app.FullName
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-?"                         -output_filename "Execute_with_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "--help"                     -output_filename "Execute_with_full_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!"                -output_filename "Execute_HelloWorld_default_font_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|banner"      -output_filename "Execute_HelloWorld_font_banner_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|big"         -output_filename "Execute_HelloWorld_font_big_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|doom"        -output_filename "Execute_HelloWorld_font_doom_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|larry3d"     -output_filename "Execute_HelloWorld_font_larry3d_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|mini"        -output_filename "Execute_HelloWorld_font_mini_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|script"      -output_filename "Execute_HelloWorld_font_script_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|small"       -output_filename "Execute_HelloWorld_font_small_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|standard"    -output_filename "Execute_HelloWorld_font_standard_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|straight"    -output_filename "Execute_HelloWorld_font_straight_produces_expected_output"
}

# Generate For : GuidGenerator
$app_name = "GuidGenerator"
$app = Search-AppByName -apps $apps -app_name $app_name
if ( $null -ne $app ) {
    $current_app_version = (Invoke-CaptureOutput -app_full_path $app.FullName -arguments "-!").StdOut.Trim()

    Clear-ExpectedOutput -app_full_name $app.FullName
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-?"                         -output_filename "Execute_with_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "--help"                     -output_filename "Execute_with_full_help_request_produces_arguments_list"
    # TODO
}

# Generate For : PauseN
$app_name = "PauseN"
$app = Search-AppByName -apps $apps -app_name $app_name
if ( $null -ne $app ) {
    $current_app_version = (Invoke-CaptureOutput -app_full_path $app.FullName -arguments "-!").StdOut.Trim()

    Clear-ExpectedOutput -app_full_name $app.FullName
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-?"                                 -output_filename "Execute_with_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "--help"                             -output_filename "Execute_with_full_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-t|1"                               -output_filename "Execute_for_1_second_default_message"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-t|5"                               -output_filename "Execute_for_5_seconds_default_message"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-t|5|Pausing for {timeout} seconds" -output_filename "Execute_for_5_seconds_custom_message"
}

# Generate For : ShowDateTime
$app_name = "ShowDateTime"
$app = Search-AppByName -apps $apps -app_name $app_name
if ( $null -ne $app ) {
    $current_app_version = (Invoke-CaptureOutput -app_full_path $app.FullName -arguments "-!").StdOut.Trim()

    Clear-ExpectedOutput -app_full_name $app.FullName
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-?"                                 -output_filename "Execute_with_help_request_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "--help"                             -output_filename "Execute_with_full_help_request_produces_command_list"
}

# Generate For : Stopwatch
$app_name = "Stopwatch"
$app = Search-AppByName -apps $apps -app_name $app_name
if ( $null -ne $app ) {
    $current_app_version = (Invoke-CaptureOutput -app_full_path $app.FullName -arguments "-!").StdOut.Trim()

    $dataFileName = New-TempFileName
    Remove-Item-IfExists $dataFileName

    Clear-ExpectedOutput -app_full_name $app.FullName
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-?"                                 -output_filename "Execute_with_help_request_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "--help"                             -output_filename "Execute_with_full_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "List|-?"                            -output_filename "Execute_with_help_request_for_List_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Start|-?"                           -output_filename "Execute_with_help_request_for_Start_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Stop|-?"                            -output_filename "Execute_with_help_request_for_Stop_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Cancel|-?"                          -output_filename "Execute_with_help_request_for_Cancel_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Elapsed|-?"                         -output_filename "Execute_with_help_request_for_Elapsed_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Pause|-?"                           -output_filename "Execute_with_help_request_for_Pause_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Purge|-?"                           -output_filename "Execute_with_help_request_for_Purge_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Resume|-?"                          -output_filename "Execute_with_help_request_for_Resume_produces_arguments_list"

    $dataFileName = New-TempFileName -FilePrefix "stopwatch" -FileExtension "dat"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Start|bob|-df|$($dataFileName)"     -output_filename "Execute_Start_new_timer"
    Remove-Item-IfExists $dataFileName

    $dataFileName = New-TempFileName -FilePrefix "stopwatch" -FileExtension "dat"
    Invoke-CaptureOutput -App_full_path $app.FullName -Arguments "Start|bob|-df|$($dataFileName)"
    Start-Sleep -Seconds 2
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Stop|bob|-df|$($dataFileName)"     -output_filename "Execute_Stop_existing_timer"
    Remove-Item-IfExists $dataFileName

    $dataFileName = New-TempFileName -FilePrefix "stopwatch" -FileExtension "dat"
    Invoke-CaptureOutput -App_full_path $app.FullName -Arguments "Start|bob|-df|$($dataFileName)"
    Start-Sleep -Seconds 2
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Cancel|bob|-df|$($dataFileName)"     -output_filename "Execute_Cancel_existing_timer"
    Remove-Item-IfExists $dataFileName

    $dataFileName = New-TempFileName -FilePrefix "stopwatch" -FileExtension "dat"
    Invoke-CaptureOutput -App_full_path $app.FullName -Arguments "Start|bob|-df|$($dataFileName)"
    Start-Sleep -Seconds 2
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Elapsed|bob|-df|$($dataFileName)"     -output_filename "Execute_Elapsed_existing_timer"
    Remove-Item-IfExists $dataFileName

    $dataFileName = New-TempFileName -FilePrefix "stopwatch" -FileExtension "dat"
    Invoke-CaptureOutput -App_full_path $app.FullName -Arguments "Start|bob|-df|$($dataFileName)"
    Start-Sleep -Seconds 2
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Pause|bob|-df|$($dataFileName)"     -output_filename "Execute_Pause_existing_timer"
    Remove-Item-IfExists $dataFileName

    $dataFileName = New-TempFileName -FilePrefix "stopwatch" -FileExtension "dat"
    Invoke-CaptureOutput -App_full_path $app.FullName -Arguments "Start|bob|-df|$($dataFileName)"
    Start-Sleep -Seconds 2
    Invoke-CaptureOutput -App_full_path $app.FullName -Arguments "Pause|bob|-df|$($dataFileName)"
    Start-Sleep -Seconds 2
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "Resume|bob|-df|$($dataFileName)"     -output_filename "Execute_Resume_existing_timer"
    Remove-Item-IfExists $dataFileName
}

Write-Host "DONE: Expected Output Text Population Complete." -ForegroundColor Green
