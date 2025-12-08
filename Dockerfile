FROM ubuntu:22.04 AS build

ARG libdragon_dir=/media/libdragon
ARG retro_crypto_dir=/media/retro-crypto

RUN apt update \
	&& apt install -y --no-install-recommends openssl \
	ca-certificates \
	wget \
	git \
	build-essential

RUN wget https://github.com/DragonMinded/libdragon/releases/download/toolchain-continuous-prerelease/gcc-toolchain-mips64-amd64.deb \
	&& apt install ./gcc-toolchain-mips64-amd64.deb \
	&& rm ./gcc-toolchain-mips64-amd64.deb

ENV N64_INST=/opt/libdragon

RUN git clone https://github.com/DragonMinded/libdragon ${libdragon_dir} \
	&& cd ${libdragon_dir} \
	&& ./build.sh \
	&& rm -r ${libdragon_dir}


RUN mkdir -p ${retro_crypto_dir}

WORKDIR "${retro_crypto_dir}"

COPY "./src" "./src"

COPY "./tests" "./tests"

COPY "./Makefile" "./"

RUN make n64

RUN mv ./bin/retro-crypto-n64.z64 ./retro-crypto_n64.z64

RUN make cli

FROM scratch AS bin_image

ARG retro_crypto_dir=/media/retro-crypto

COPY --from=build ${retro_crypto_dir}/retro-crypto_n64.z64 /retro-crypto_n64.z64

COPY --from=build ${retro_crypto_dir}/bin/retro-crypto /retro-crypto_linux-x86_64
