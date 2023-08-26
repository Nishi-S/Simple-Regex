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
# *+
expect "a*a+" "a" "a"
expect -notMatch "a*a+" ""
expect "k(a*k+)*k" "kkakkk" "kkakkk"
# \w \s \d
expect "\w+" "Hello123" "Hello123"
expect "\w+" "Good_Morning" "Good_Morning"
expect "\s+" " `t" " `t"
expect "\d+" "12345" "12345"
expect -notMatch "\d+" "a345" 
expect "\d+" "13 45" "13"