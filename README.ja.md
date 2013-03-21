jamruby
====

__jamruby__はJavaとmrubyをブリッジするためのライブラリです。

ビルドの準備
----

1. Githubからソースコードを取得します。
2. 'mruby/build_config.rb'をAndroid NDKでのクロスビルド用に編集します。
3. 'mruby'を対象のABI(armeabi, armeabi-v7a, x86, mips)にあわせてビルドします。
4. 'jamruby'をAndroid NDKを使用して共有ライブラリとしてビルドします。

----
    $ git clone https://github.com/mruby/mruby.git
    $ git clone https://github.com/jamruby/jamruby.git
    $ cd mruby
    $ vi build_config.rb
    $ ANDROID_NDK_HOME="/path/to/your/NDK" make
    $ cd ../

'mruby'のビルドには'jamruby'が必要になります。<br/>
また、'mruby'と'jamruby'は同一のディレクトリに配置されていなければなりません。<br/>
もしあなたの環境がそうでなければ、ご自分で'jamruby/jni/Android.mk'を編集してください。

----
以下のようなコードを'mruby/build_config.rb'に追加します。

build_config.rb:<br/>
<pre>
MRuby::CrossBuild.new('android-armeabi') do |conf|
  ENV['ANDROID_TARGET_ARCH'] = 'arm'
  ENV['ANDROID_TARGET_ARCH_ABI'] = 'armeabi'
  toolchain :androideabi
end
MRuby::CrossBuild.new('android-armeabi-v7a') do |conf|
  ENV['ANDROID_TARGET_ARCH_ABI'] = 'armeabi-v7a'
  toolchain :androideabi
end
MRuby::CrossBuild.new('android-x86') do |conf|
  ENV['ANDROID_TARGET_ARCH'] = 'x86'
  toolchain :androideabi
end
</pre>

JNI libraryのビルドの方法
----

'ndk-build'コマンドを使用します。

    $ cd jamruby/jni
    $ ndk-build

Eclipseでプロジェクトを開く
----

1. プロジェクトをワークスペースへインポートします。
2. Githubからcloneした'jamruby'をルートディレクトリに指定してインポートします。
3. プロジェクトをビルドします。
4. 'jamruby'はAndroidのLibraryプロジェクトとしてビルドされます。

ライセンス
----

__MIT License__

Copyright (c) 2012-2013 jamruby developers

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

