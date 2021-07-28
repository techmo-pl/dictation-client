FROM python:3.6-slim

ARG DEBIAN_FRONTEND=noninteractive

ADD ./python /dictation_client_python

WORKDIR /dictation_client_python

RUN ./setup.sh -y

RUN apt-get clean \
	&& rm -fr /var/lib/apt/lists/* \
	&& rm -fr /var/cache/apt/* 

ADD ./tools/pulseaudio.docker.conf /etc/pulse/client.conf
