FROM registry.gitlab.steamos.cloud/steamrt/scout/sdk:latest

WORKDIR /root

LABEL "com.azure.dev.pipelines.agent.handler.node.path"="/usr/local/bin/node"

RUN apt update -y && apt upgrade -y && apt install -y software-properties-common && apt install -y curl build-essential unzip

COPY setup_python3.sh setup_node.sh ./
RUN chmod +x setup_python3.sh && ./setup_python3.sh && rm -f setup_python3.sh
RUN chmod +x setup_node.sh && ./setup_node.sh && rm -f setup_node.sh
