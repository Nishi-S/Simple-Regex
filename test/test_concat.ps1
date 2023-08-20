$compiler = Get-ChildItem ".\x64\Release\*compile*.exe" 
$vm = Get-ChildItem ".\x64\Release\*vm*.exe" 

function expect
{
    param (
        $regex,
        $text,
        $expect,
        [switch]$notMatch
    )
    
    Write-Host "`"$regex`" => `"$text`" " -NoNewline
    $ret = &$compiler $regex | &$vm $text
    if ($ret -ne $expect)
    {
        Write-Host "NG"
    }
    else
    {
        Write-Host "OK" 
    }
}

Write-Host "============ concat test ============"
expect "applebanana" "applebanana" "applebanana"
expect "" "" ""
expect -notMatch "aaa" "abc" 
