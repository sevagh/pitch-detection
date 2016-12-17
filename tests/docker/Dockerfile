FROM ubuntu
MAINTAINER sevag.hanssian@gmail.com

RUN apt update -y
RUN apt install -y gcc g++ make cmake pkg-config

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
