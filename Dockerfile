FROM debian:jessie

RUN apt-get update
RUN apt-get install -y --force-yes build-essential nasm gcc make gdb vim python git
