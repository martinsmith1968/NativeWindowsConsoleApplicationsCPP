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
    $file_name_only = [System.IO.Path]::GetFileNameWithoutExtension($file.Name)
    $apps[$file_name_only] = $file
}

function Search-AppByName {
    param (
        [string]$app_name
    )
    Write-Host "Looking for app : $($app_name)..." -ForegroundColor Yellow
    if ( $apps.ContainsKey($app_name) ) {
        return $apps[$app_name]
    }
    return $null
}


function Set-ExpectedOutput {
    param (
        [string]$app_full_path,
        [string]$arguments,
        [string]$output_filename,
        [string]$expected_output_path = $null
    )

    $app_name = [System.IO.Path]::GetFileNameWithoutExtension($app_name)

    if ( [string]::IsNullOrEmpty($expected_output_path) ) {
        $expected_output_path = Join-Path -Path $PSScriptRoot -ChildPath ".." "tests" "apps" ($app_name + ".Tests") "Expectedoutput"
    }

    $parameters = $arguments.Split("|")

    Write-Host "  $($app_name) - Generating : $($output_filename)"
    & $app_full_path $parameters | Set-Content -Path (Join-Path -Path $expected_output_path -ChildPath "$($output_filename).example") -Encoding UTF8
}


# Generate For : BannerText
$app_name = "BannerText"
$app = Search-AppByName -app_name $app_name
if ( $null -ne $app ) {
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-?"                             -output_filename "Execute_with_help_request_produces_arguments_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "bob"                            -output_filename "Execute_with_text_only_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "bob|-minl|80"                   -output_filename "Execute_with_text_and_min_length_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "a|bb|ccc|dddd|eeeee"            -output_filename "Execute_with_multiple_text_lines_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "a|bb|ccc|dddd|eeeee|-ta|Center" -output_filename "Execute_with_multiple_text_lines_aligned_center_produces_expected_output"
}


# Generate For : Stopwatch
$app_name = "Stopwatch"
$app = Search-AppByName -app_name $app_name
if ( $null -ne $app ) {
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "-?"         -output_filename "Execute_with_help_request_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "cancel|-?"  -output_filename "Execute_command_cancel_with_help_request_short_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "elapsed|-?" -output_filename "Execute_command_elapsed_with_help_request_short_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "list|-?"    -output_filename "Execute_command_list_with_help_request_short_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "pause|-?"   -output_filename "Execute_command_pause_with_help_request_short_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "resume|-?"  -output_filename "Execute_command_resume_with_help_request_short_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "start|-?"   -output_filename "Execute_command_start_with_help_request_short_produces_command_list"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "stop|-?"    -output_filename "Execute_command_stop_with_help_request_short_produces_command_list"
}


# Generate For : FigLetText
$app_name = "FigLetText"
$app = Search-AppByName -app_name $app_name
if ( $null -ne $app ) {
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!"             -output_filename "Execute_HelloWorld_default_font_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|banner"   -output_filename "Execute_HelloWorld_font_banner_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|big"      -output_filename "Execute_HelloWorld_font_big_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|doom"     -output_filename "Execute_HelloWorld_font_doom_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|larry3d"  -output_filename "Execute_HelloWorld_font_larry3d_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|mini"     -output_filename "Execute_HelloWorld_font_mini_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|script"   -output_filename "Execute_HelloWorld_font_script_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|small"    -output_filename "Execute_HelloWorld_font_small_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|standard" -output_filename "Execute_HelloWorld_font_standard_produces_expected_output"
    Set-ExpectedOutput -app_full_path $app.FullName -arguments "HelloWorld!|-n|straight" -output_filename "Execute_HelloWorld_font_straight_produces_expected_output"
}

Write-Host "DONE: Expected Output Text Population Complete." -ForegroundColor Green
