FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

ARG CUR_GID
ARG CUR_UID
ARG CUR_GN
ARG CUR_UN

COPY packages.txt .

RUN apt-get update \
	&& xargs -a packages.txt apt-get install -y \
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*

#ENV LC_ALL=en_US.UTF-8
#ENV LANGUAGE=en_US:en
#ENV LANG=en_US.UTF-8		
#RUN locale-gen en_US.UTF-8

RUN groupadd -g ${CUR_GID} ${CUR_GN} \
	&& useradd -u ${CUR_UID} -g ${CUR_GN} -d /home/${CUR_UN} ${CUR_UN} \
	&& passwd -d ${CUR_UN} \
	&& usermod -a -G sudo ${CUR_UN} \
	&& mkdir /home/${CUR_UN} \
	&& chown -R ${CUR_UN}:${CUR_GN} /home/${CUR_UN}
	
WORKDIR /home/${CUR_UN}

USER ${CUR_UN}

