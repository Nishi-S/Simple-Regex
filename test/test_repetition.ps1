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

Write-Host "============ repetition test ============"
# *
expect "a*" "aa" "aa"
expect "a*" "" ""
expect -notMatch "aa*" ""
expect "a*" "abc" "a"
expect "k(abc)*k" "kabcabck" "kabcabck"
expect "k(abc)*k" "kk" "kk"
# +
expect "a+" "aa" "aa"
expect -notMatch "a+" ""
expect "a+" "abc" "a"
expect "k(abc)+k" "kabcabck" "kabcabck"
expect -notMatch "k(abc)+k" "kk"
# ?
expect "aa?" "a" "a"
expect "a?" "" ""
expect -notMatch "aa?" ""
expect "a?" "abc" "a"
expect "k(abc)?k" "kabck" "kabck"
expect "k(abc)?k" "kk" "kk"