# Docker Images for Testing Storage and Memory Transport Technologies

These Docker images are designed to facilitate testing and development of storage or memory transport technologies, including DAOS, direct GPU memory transport, and data processing algorithms. They are specialized for use with CLAS12 raw data format and ESnet load balancing technology.

## Images

### Source Image: clas-ejfat:v0.1

#### Environment Variables

- `$CLAS_FILE`: Specifies the path to the CLAS12 raw data file. If not set, the default raw data file from the CLAS12 EG1 experiment within the image will be used.
- `$LB_IP`: IP address of the FPGA accelerated load balancer. The default is set to the ESnet load balancer.
- `$LB_HOST`: IP of the LB control plane host computer. The default is the LB host at ESnet. For using local LB hardware, contact Mike for the control plane address.
- `$MUT`: Maximum packet size, with a default of 9000 Bytes.
- `$BUFDELAY`: Buffer stream delay in microseconds, with a default set to 2000 microseconds (2 milliseconds).

### Sync Image: ejfat-et:v0.1

#### Environment Variables

- `$PR_HOST`: IP address of the processing host. Defaults to the IP address where the container is running.
- `$LB_HOST`: IP of the LB control plane host computer. The default is the LB host at ESnet. For using local LB hardware, contact Mike for the control plane address.

## Requirements

To ensure proper functionality and use of these containers, the source and the sync must be connected through the ESnet load balancer. This setup is specific to the data format and transport technology used in these images.

## Usage

Before running the images, ensure you have Docker installed and running on your system. You can pull and run the images from Docker Hub using the following commands:

### Source Image

```bash
docker pull <docker-hub-username>/clas-ejfat:v0.1
docker run -e CLAS_FILE=<path_to_clas12_raw_data_file> -e LB_IP=<load_balancer_ip> -e LB_HOST=<load_balancer_host_ip> -e MUT=<maximum_packet_size> -e BUFDELAY=<buffer_delay> <docker-hub-username>/clas-ejfat:v0.1
```

### Sync Image

```bash
docker pull <docker-hub-username>/ejfat-et:v0.1
docker run -e PR_HOST=<processing_host_ip> -e LB_HOST=<load_balancer_host_ip> <docker-hub-username>/ejfat-et:v0.1
```

Replace `<docker-hub-username>`, `<path_to_clas12_raw_data_file>`, `<load_balancer_ip>`, `<load_balancer_host_ip>`, `<maximum_packet_size>`, and `<processing_host_ip>` with your specific values.

## Support

For assistance with setting up or using these Docker images, please contact Mike for specific inquiries related to the load balancer setup and other technical support.

