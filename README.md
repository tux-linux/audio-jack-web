# audio-jack-web

Or a way to browse the Internet between two computers with only two 3.5mm audio cables and some Morse/POCSAG fanciness.

## How does it work?
Remember the days when you had to hook up a telephone line to a modem to be able to access the Internet (I did not have the chance to live in those days, but well...)? Welcome back. This makes use of two simple reversible data channels that connect computers together: 3.5mm audio cables. The client sends a webpage request, and the server answers this request by sending the compressed HTML code of this webpage over... sound.

### Request
You type in your URL in the basic browser's address bar, and then you click `Go`. Some handy [script](https://github.com/tux-linux/audio-jack-web/blob/main/scripts/client/request.sh) handles conversion shenanigans for you, then plays out loud base32-encoded Morse over your `Line Out` port for the server to understand that you want to access a webpage with a special `++END++` string at the end to tell the server your computer has finished sending its request.
### Receive
The server decodes the base32 string into `wget`-understandable format and calls his [send script](https://github.com/tux-linux/audio-jack-web/blob/main/scripts/server/send.sh) with that URL as its first argument. `wget` first fetches the webpage's HTML code, then the script compresses it into XZ data and encodes it to base64, then breaks it down in several chunks to accommodate the needs of the [POCSAG](https://www.sigidwiki.com/wiki/POCSAG) encoder, which is limited to around 512 characters/chunk. Each chunk is passed, one by one, to that encoder and is thrown out of the server's `Line Out` port, to the `Line In` port of the client at a glorious 2400 bits per second data rate.

The client receives this data, accompanied by a transmission header (containing the number of chunks to be transmitted and the total size of the data) and trailer (containing the `==END==` signal), assembles all chunks, decodes base64, uncompresses XZ data, and displays the webpage into QWebEngineView.

## How do I try this?
### Requirements
- Two Linux PCs (preferably running a Debian-based system), both with a `Line In` and a `Line Out` port
- The "client" PC must be running a GUI for the Qt app to run
- Good enough audio cables
- An Internet connection for the server to retrieve webpages; not necessary on the client
- Time & patience

### Setting it up
#### Apply to both PCs
Clone the Git repository in a terminal:
```
git clone --recurse-submodules https://github.com/tux-linux/audio-jack-web
```
`cd` into the repository and execute the [bootstrap.sh](https://github.com/tux-linux/audio-jack-web/blob/main/bootstrap.sh) script to compile the required binaries:
```
./bootstrap.sh
```
If you're not running a Debian-based system, build instructions are pretty much the same, only dependencies to install will change.

#### Server PC
Play around with audio configuration & volume in `pavucontrol`, for me it works best like this:
![Output](https://github.com/tux-linux/audio-jack-web/raw/main/images/howto/server/pavucontrol_output.png)
![Input](https://github.com/tux-linux/audio-jack-web/raw/main/images/howto/server/pavucontrol_input.png)

`cd` into `scripts/server` then launch the server daemon:
```
./daemon.sh
```
**Important**: this script must *always* be run from its initial directory, otherwise it won't work properly.

#### Client PC
Play around with audio configuration & volume in `pavucontrol`, for me it works best like this:
![Output](https://github.com/tux-linux/audio-jack-web/raw/main/images/howto/client/pavucontrol_output.png)
![Input](https://github.com/tux-linux/audio-jack-web/raw/main/images/howto/client/pavucontrol_input.png)

`cd` into `audio-jack-web-qt`, then launch the Qt application:
```
./audio-jack-web-qt
```
**Important**: this app must *always* be run from its build directory, otherwise it won't work properly.

#### Have fun!
Enter something to search with [FrogFind](http://frogfind.com), or type an URL into the address bar, then strike Return. Now, wait. Something should be happening between the two computers and soon enough (yeah... it's slow ;p), you'll see a webpage appearing on your client's browser.
![FrogFind demo](https://github.com/tux-linux/audio-jack-web/raw/main/images/demo/demo_1.png)
![GitHub demo](https://github.com/tux-linux/audio-jack-web/raw/main/images/demo/demo_2.png)
**Note**: if the first webpage request doesn't transmit well (there might be strange characters in the URL on the server's end due to some corruption), try again and it should work better.

## Licensing
All the files in this project are licensed under the GNU General Public License version 3.0, unless otherwise specified (e.g. in submodules).
