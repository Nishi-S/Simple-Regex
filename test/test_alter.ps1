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

Write-Host "============ alternation test ============"
expect "(a|b)+" "abaabbbaa" "abaabbbaa"
expect "(apple)|(banana)" "apple" "apple"
expect "(apple)|(banana)" "banana" "banana"
expect -notMatch "(apple)|(banana)" "orange"
expect "\d+|a\d*" "123" "123"
expect "\d+|a\d*" "a456" "a456"
expect "\d+|a\d*" "abc" "a"
