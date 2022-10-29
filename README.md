# TFT000001_for_A

このプログラムはTFT000001のArduino移植版です。    
Teensy 4.xやRaspberry Pi Picoを使ってILI9341とILI9486のLCDにグラフィックスを描画するプログラムです。    
まだ、文字表示には対応してません。    
一応Arduino互換機一般にも対応してます(ただし、AVRのArduinoでは動作しません)。    
Teensy 4.xで使いたくてArduinoに移植したのでRaspberry Pi PicoとArduino互換機一般への対応はおまけです。    
(TFT000001はもともとRaspberry Pi Picoのpico-sdk用のライブラリなのでおまけとはいってもそれなりの対応にはなってると思います。)    
    
Raspberry Pi Picoのみ直接レジスタアクセスでSPIを操作するモードを追加しました。    
Raspberry Pi Picoの場合、ArduinoのSPIの関数ででSPIを操作すると非常に遅いので    
直接レジスタアクセスでSPIを操作する動作モードに対応する関数の数を増やしています。    
(pico-sdk版ではdrawPixelのみ直接レジスタアクセスでSPIを操作するモードに対応していました)    
特にArduino公式のMbedベースのRaspberry Pi Pico用のArduinoはSPIの速度が遅いのでかなり速くなります。    
(examplesでは直接レジスタアクセスでSPIを操作するモードは無効になっているので手動でTFT000001_config.hで設定してください)    
    
ILI9486ではTeensy 4.xやRaspberry Pi Picoを使った場合にSPIの他に8bitパラレルに対応してます。    
ILI9486で8bitパラレルを使った場合は16bitカラーと18bitカラーに対応してます    
ILI9486でSPIを使った場合は18bitカラーのみの対応です。    
ILI9341ではSPIのみの対応で16bitカラーになります。    
    
Teensy 4.xの8bitパラレルでは2種類のピン設定の中から選択する形で使用するピンは固定になります。    
Raspberry Pi Picoの8bitパラレルではD0で指定したピンから連続する8本のピンが    
8bitパラレルのbit0からbit7に対応します。    
    
TFT000001_config.hでLCDの種類、機種やインターフェースの種類などを    
コメントアウトしたりコメントアウトを外したりして設定をしてから利用する必要があります。    
    
examplesでILI9486、ILI9341の種類や機種、インターフェースによって    
それぞれに対応したTFT000001_config.hを作成してありますので    
そのTFT000001_config.hの内容をライブラリのフォルダのsrcフォルダへコピーしてください。    
    
接続に関してはILI9486の8bitパラレルではLCD側のRDはVCCへ、LCD側のCSはGNDへ接続してください。    
また、WRが8bitパラレルのbit0からbit7の信号線やDCの信号線などと干渉しやすいようなので    
配線やピン配置には注意が必要なようです。    
ILI9486のSPIでの接続ではWRがSCKに相当するようです。    
LCDによってはDCピンの名前がRSとなっている場合もあります。    
Teensy 4.xに関してはTeensy4.x自体の消費電力が多いためか    
VCCやGNDにノイズが乗りやすいようなのでVCCやGNDの接続は念入りに太くするようにした方がいいようです。    
