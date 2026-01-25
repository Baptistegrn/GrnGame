# MANYLINUX 2.28 (Basé sur RHEL/AlmaLinux 8)
# GLIBC 2.28 -> Compatible Ubuntu 20.04+, Debian 10+, RHEL 8+
FROM quay.io/pypa/manylinux_2_28_x86_64:latest

LABEL description="Build Manylinux (Glibc 2.28 / Ubuntu 20.04+ compatible)"

# 1. DEPENDANCES SYSTEME (yum au lieu de apt)
# On installe les headers de développement pour que SDL2
# puisse activer le son (ALSA/Pulse) et la fenêtre (X11).
RUN yum update -y && \
    yum install -y \
    # Outils de base
    gcc-c++ make git unzip wget \
    # --- AUDIO (TRES IMPORTANT POUR SDL_MIXER) ---
    alsa-lib-devel \
    pulseaudio-libs-devel \
    # --- VIDEO / FENETRAGE (X11) ---
    libX11-devel \
    libXext-devel \
    libXrandr-devel \
    libXinerama-devel \
    libXcursor-devel \
    libXi-devel \
    # --- OPENGL ---
    mesa-libGL-devel \
    # --- CLAVIER ---
    libxkbcommon-devel \
    # --- PYTHON (Requis par certains scripts de build internes, xmake gère le reste) ---
    python3 \
    && yum clean all

# 2. INSTALLATION XMAKE
ENV XMAKE_ROOT=y
RUN curl -fsSL https://xmake.io/shget.text | bash

# Ajout au PATH
ENV PATH="/root/.local/bin:${PATH}"

# Dossier de travail
WORKDIR /io

CMD ["/bin/bash"]