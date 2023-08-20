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
    
    $ret = &$compiler $regex | &$vm $text
    if ($ret -ne $expect)
    {
        Write-Host "`"$regex`" => `"$text`" " -NoNewline 
        Write-Host "expect $expect, but got $ret"
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
expect "(aaa)" "aaa" "aaa"
