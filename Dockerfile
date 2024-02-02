FROM ubuntu:20.04

# Set the PATH
ENV PATH /ersap:$PATH

COPY . ersap

WORKDIR /ersap

CMD ["cd /ersap"]

ENTRYPOINT ["run-source.sh"]
