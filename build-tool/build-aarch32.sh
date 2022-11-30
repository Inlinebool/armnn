docker build \
--build-arg BUILD_TYPE=dev \
--build-arg SETUP_ARGS="--target-arch=aarch32 --onnx-parser" \
--build-arg BUILD_ARGS="--target-arch=aarch32 --onnx-parser --neon-backend" \
--tag armnn:aarch32 \
--file docker/Dockerfile .

rm docker_output/armnn_aarch32_install.tar.gz
./scripts/docker-copy-to-host.sh armnn:aarch32 armnn_aarch32_install.tar.gz
