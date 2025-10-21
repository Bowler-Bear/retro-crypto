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
<img width="900" height="625" alt="dice_input" src="https://github.com/user-attachments/assets/539c026a-4afc-4d7b-9e4f-41b0b28f5651" />
<img width="905" height="627" alt="seed_options_menu" src="https://github.com/user-attachments/assets/1bdf5a69-314e-434c-b950-3d5658263a35" />
<img width="904" height="627" alt="addresses_menu" src="https://github.com/user-attachments/assets/3acfd05c-85eb-4a63-83e0-e45cd1fd1f6e" />
<img width="900" height="625" alt="btc_address" src="https://github.com/user-attachments/assets/cfe74d31-5d9e-4310-846a-dff44620981f" />
<img width="900" height="625" alt="doge_address" src="https://github.com/user-attachments/assets/7ec670ed-9ef9-407f-b879-102259a26cab" />
<img width="900" height="625" alt="etc_address" src="https://github.com/user-attachments/assets/761064bc-d043-41bc-8a9f-477c319465d7" />
<img width="900" height="625" alt="eth_address" src="https://github.com/user-attachments/assets/645a5d74-f973-4622-99ca-1d7dcee24a78" />
<img width="900" height="625" alt="nostr_address" src="https://github.com/user-attachments/assets/ab4afd36-b12f-448c-8375-8f7541efefff" />
<img width="900" height="625" alt="xmr_address" src="https://github.com/user-attachments/assets/e0293a3a-e6de-4c85-baea-995472ceb009" />
<img width="900" height="625" alt="output_phrases" src="https://github.com/user-attachments/assets/93f50884-7b4f-47c3-8eff-eb21319aa68c" />
<img width="900" height="625" alt="bip39_24_output" src="https://github.com/user-attachments/assets/066268c2-a3d6-4f90-9490-5ad1ce589e66" />
<img width="900" height="625" alt="english_xmr_phrase_output" src="https://github.com/user-attachments/assets/4c7e39b9-7db9-4afa-9814-794a4c66cd96" />
<img width="900" height="625" alt="french_xmr_phrase_output" src="https://github.com/user-attachments/assets/1e49cb22-efa9-4394-bc5a-cad90d4c5c5d" />
<img width="900" height="625" alt="italian_xmr_phrase_output" src="https://github.com/user-attachments/assets/6151904e-b227-45a2-bfa2-45e27b92e3f8" />
<img width="900" height="625" alt="dutch_xmr_phrase_output" src="https://github.com/user-attachments/assets/758943c2-1f9c-4781-bacc-f0808abc89da" />
<img width="900" height="625" alt="portuguese_xmr_phrase_output" src="https://github.com/user-attachments/assets/36de1c05-d9f2-45f2-aba2-aec6c821d5a2" />
<img width="900" height="625" alt="esperanto_xmr_phrase_output" src="https://github.com/user-attachments/assets/f3976f48-7410-4cfd-b23b-693df36d11f1" />
<img width="900" height="625" alt="qr_code" src="https://github.com/user-attachments/assets/b23926f4-be17-4c9a-9f01-8874e99f433a" />
<img width="902" height="626" alt="vanity_address" src="https://github.com/user-attachments/assets/33d94753-93e6-4590-9b16-dc8f58a59629" />
<img width="900" height="625" alt="input_phrases" src="https://github.com/user-attachments/assets/10402816-4770-4c7d-a1e8-ce1e3864d13c" />


## Donations ##
If you found this project interesting or want to help fund it's continued development, consider donating. Donations can be sent to 85rkddSoYqwN4neAAgjJTFYYgyfU6pwT9dYquNyBzdtqBnr8QZCU1qoR6kjwMWEF6EMd2aGtrCuuA19irjWbTbKSKKF5yuv.
