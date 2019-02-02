For windows build on debian/ubuntu through [MXE](https://mxe.cc/):

```
sudo apt-get install \
    autoconf \
    automake \
    autopoint \
    bash \
    bison \
    bzip2 \
    flex \
    g++ \
    gettext \
    git \
    gperf \
    intltool \
    libc6-dev-i386 \
    libgdk-pixbuf2.0-dev \
    libltdl-dev \
    libssl-dev \
    libtool-bin \
    libxml-parser-perl \
    lzip \
    make \
    openssl \
    p7zip-full \
    patch \
    perl \
    pkg-config \
    python \
    ruby \
    sed \
    unzip \
    wget \
    xz-utils
sudo apt-get install g++-multilib

cd ..
git clone https://github.com/mxe/mxe.git
cd mxe
make qtbase
```

Then, to finalize build:
```
export PATH=../mxe/usr/bin/:$PATH
i686-w64-mingw32.static-qmake-qt5
make
./package/windows/release_win_pkg.sh
```

