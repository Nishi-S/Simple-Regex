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
        if ($notMatch)
        {
            Write-Host "expect not matched, but got $ret"
        }
        else
        {
            Write-Host "expect $expect, but got $ret"
        }
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
expect "\d" "4" "4"
expect "\d\d\d" "590" "590"
expect "\w" "_" "_"
expect "\s" " " " "
expect "\\" "\aa" "\"
expect "\\\d" "\0a" "\0"
expect "\*\+\?\|\(\)\[\]" "*+?|()[]" "*+?|()[]"