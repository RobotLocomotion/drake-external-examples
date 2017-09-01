FROM ubuntu:16.04
RUN apt-get update -qq \
  && apt-get install -qy --no-install-recommends \
    ca-certificates \
    git \
    openssh-client \
  && rm -rf /var/lib/apt/lists/*
