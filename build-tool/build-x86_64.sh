docker build \
--build-arg BUILD_TYPE=dev \
--build-arg SETUP_ARGS="--target-arch=x86_64 --onnx-parser" \
--build-arg BUILD_ARGS="--target-arch=x86_64 --onnx-parser --ref-backend" \
--tag armnn:x86_64 \
--file docker/Dockerfile .

rm docker_output/armnn_x86_64_install.tar.gz
./scripts/docker-copy-to-host.sh armnn:x86_64 armnn_x86_64_install.tar.gz
