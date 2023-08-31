VM型の正規表現エンジンをCで実装したプロジェクトです。

参考文献：
[正規表現技術入門――最新エンジン実装と理論的背景](https://gihyo.jp/book/2015/978-4-7741-7270-5)

# プロジェクト構造
正規表現のパターンを VM の命令列（Asssembler）に変換する regex-compile と、
命令列から、入力文字列とマッチする部分文字列を参照する regex-vm との２つのプロジェクトに分かれています。
![プロジェクト構造](https://github.com/Nishi-S/Simple-Regex/assets/70260859/d663bff9-0091-459a-a595-245038413230)

# ビルド
ビルドには Visual Studio を用いています。
ビルドすると、regex-vm.exe と regex-compile.exe の2つの実行ファイルが作成されます。

# 文法
以下の正規表現文法に対応しています。
| 要素             | 説明                                           |
|-----------------|----------------------------------------------|
| `*`             | 直前の要素が0回以上繰り返しにマッチ           |
| `+`             | 直前の要素が1回以上繰り返しにマッチ           |
| `?`             | 直前の要素が0回または1回にマッチ              |
| `[abc]`         | `a`、`b`、または`c`のいずれかにマッチ         |
| `[a-z]`         | `a`から`z`までの文字にマッチ                 |
| `\d`            | 任意の数字にマッチ                             |
| `\w`            | 任意の単語文字（アルファベット・数字・アンダースコア）にマッチ |
| `\s`            | 任意の空白文字にマッチ                         |
| `(abc)`         | グループ化し、`abc`にマッチ                   |
| `a\|b`          | `a`または`b`にマッチ                          |

# 使用方法
'''
PS > .\regex-compile.exe "a*bc" | .\regex-vm.exe "aabcdef"
aabc
'''
'''
PS > .\regex-compile.exe "(a|b)*" | .\regex-vm.exe "aba"
aba
'''
