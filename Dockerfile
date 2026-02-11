#ubuntu 20.04+ 
FROM quay.io/pypa/manylinux_2_28_x86_64:latest
LABEL description="Build Manylinux"
RUN yum update -y && \
    yum install -y \
    gcc-c++ make git unzip wget \
    alsa-lib-devel \
    pulseaudio-libs-devel \
    #video
    libX11-devel \
    libXext-devel \
    libXrandr-devel \
    libXinerama-devel \
    libXcursor-devel \
    libXi-devel \
    #opengl
    mesa-libGL-devel \
    #clavier
    libxkbcommon-devel \
    #python3
    python3 \
    && yum clean all
#xmake
ENV XMAKE_ROOT=y
RUN curl -fsSL https://xmake.io/shget.text | bash
#path
ENV PATH="/root/.local/bin:${PATH}"
# Dossier de travail
WORKDIR /io

CMD ["/bin/bash"]