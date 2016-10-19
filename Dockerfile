FROM fedora 
MAINTAINER sevag.hanssian@gmail.com

ADD entrypoint.sh /
RUN dnf install -y fftw fftw-devel cmake make gcc gcc-c++

ENTRYPOINT ["/entrypoint.sh"]
