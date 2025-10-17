# Retro-Crypto #
A multi coin crypto seed and address library meant to be easily ported to retro game systems. The idea for this project comes from [this post](https://habla.news/a/naddr1qvzqqqr4gupzqamkcvk5k8g730e2j6atadp6mxk7z4aaxc7cnwrlkclx79z4tzygqqxkwctdv4cxzerhv9kxcet5jlnqmv) by **whoever loves Digit**.


## Dependencies ##
Install packages for compiling by running the following commands:

`sudo apt update`

`sudo apt install build-essential`

To build for nintendo 64, follow the instructions [here](https://github.com/DragonMinded/libdragon/wiki/Installing-libdragon) to install libdragon.


## CLI Build ##
To build for linux **command line interface**, run:

`make cli`

The resulting binary will be in the bin folder.


## N64 Build ##
To build a binary for the **nintendo 64**, run:

`make n64`

The resulting z64 file will be in the bin folder.


### Importing Private Keys into [Bitcoin Core Wallet](https://github.com/bitcoin/bitcoin) ###
Under the Window menu, select 'Console'. Enter in the command:

`getdescriptorinfo "pkh(<WIF private key>)"`

where \<WIF private key\> is the WIF private key from the bitcoin address page. Then run the command:

`importdescriptors '[{"desc":"pkh(<WIF private key>)#<checksum>","timestamp","now"}]'`

where \<checksum\> is the checksum value output by the previous command.

<img width="198" height="190" alt="bitcoin_core_file_menu_console_highlighted" src="https://github.com/user-attachments/assets/1457ed70-6408-4d7f-818e-0b0cffda7fd9" />
<br/>
<img width="767" height="562" alt="bitcoin_core_key_import" src="https://github.com/user-attachments/assets/177a5487-ee80-447b-ba65-2a1ab73cb13e" />


### Importing Private Keys into [Doge Core Wallet](https://github.com/dogecoin/dogecoin) ###
Under the File menu, select 'Import Private Key...' then enter the WIF private key from the Doge address page.

<img width="278" height="285" alt="doge_core_file_menu_import_highlighted" src="https://github.com/user-attachments/assets/a76a4514-e064-4677-91a1-40f5ab106ee2" />
<br/>
<img width="559" height="253" alt="doge_core_private_key_import" src="https://github.com/user-attachments/assets/70aab3d9-529c-42f5-980a-fb6a4c8ca317" />


### Creating a Monero wallet ###
To create a monero wallet using monero-wallet-cli, enter the spend key shown on the Monero adddress page after running the command:

`monero-wallet-cli --generate-from-spend-key <wallet name>`

Alternatively, enter the Legacy English Monero seed phrase after running:

`monero-wallet-cli --restore-from-seed`


### Changing the Color Scheme ###
The color scheme can be updated by modifying the existing DEFAULT_\*G_COLOR_\* definitions in the Makefile within the COLOR_DEFS variable. DEFAULT_\*G_COLOR_RED, DEFAULT_\*G_COLOR_GREEN, and DEFAULT_\*G_COLOR_BLUE each signify the RGB component vaules to be used by either the foreground/text(FG) or background(BG). They can be set to any value from 0x00 to 0xff.


## Screenshots ##
<img width="903" height="631" alt="main_menu" src="https://github.com/user-attachments/assets/06af3524-d9e4-4fec-bb37-9c61132e26fe" />
<img width="903" height="627" alt="dir_inputs" src="https://github.com/user-attachments/assets/f347f840-cd52-412d-b85a-96eceb4e9a68" />
<img width="905" height="627" alt="seed_options_menu" src="https://github.com/user-attachments/assets/1bdf5a69-314e-434c-b950-3d5658263a35" />
<img width="904" height="627" alt="addresses_menu" src="https://github.com/user-attachments/assets/3acfd05c-85eb-4a63-83e0-e45cd1fd1f6e" />
<img width="901" height="625" alt="btc_address" src="https://github.com/user-attachments/assets/fc49bba2-86e6-48e4-b3a6-95af5afba45a" />
<img width="901" height="625" alt="doge_address" src="https://github.com/user-attachments/assets/8d24455a-3393-4879-93a6-acde47d972ab" />
<img width="900" height="625" alt="etc_address" src="https://github.com/user-attachments/assets/f6494bc0-5741-45a2-a4b5-73918acb0ebb" />
<img width="900" height="625" alt="eth_address" src="https://github.com/user-attachments/assets/826ba319-cacd-44ac-aca7-eee06a5d929d" />
<img width="900" height="625" alt="nostr_address" src="https://github.com/user-attachments/assets/56d02e7f-0a72-4766-9d83-d451011e35bd" />
<img width="900" height="625" alt="xmr_address" src="https://github.com/user-attachments/assets/6c2cb493-3cc5-4ab6-9723-377d75a5e841" />
<img width="902" height="625" alt="phrases" src="https://github.com/user-attachments/assets/018cd768-b750-4112-94fc-53cbbd9b839d" />
<img width="903" height="624" alt="bip39-24_output" src="https://github.com/user-attachments/assets/c3d18afc-48af-4805-a226-295f97a2a058" />
<img width="904" height="627" alt="english_legacy_monero_phrase" src="https://github.com/user-attachments/assets/46fcea60-d454-4bf9-9e41-8ba3ca9e2e28" />
<img width="903" height="625" alt="qr_code" src="https://github.com/user-attachments/assets/16606427-3d4e-4e76-9d19-d5ddaac9890c" />
<img width="902" height="626" alt="vanity_address" src="https://github.com/user-attachments/assets/33d94753-93e6-4590-9b16-dc8f58a59629" />
<img width="902" height="628" alt="phrase_input" src="https://github.com/user-attachments/assets/f287b5f7-43b8-4c80-b9ff-e379c25970c9" />


## Donations ##
If you found this project interesting or want to help fund it's continued development, consider donating. Donations can be sent to 85rkddSoYqwN4neAAgjJTFYYgyfU6pwT9dYquNyBzdtqBnr8QZCU1qoR6kjwMWEF6EMd2aGtrCuuA19irjWbTbKSKKF5yuv.
