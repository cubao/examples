.PHONY: all reset_submodules clean

all:
	@echo nothing special

reset_submodules:
	git submodule update --init --recursive

clean:
	rm -rf build dist

DOCKER_BUILD_TAG := cubao/release
docker_test_build:
	docker run --rm -v `pwd`:/workdir \
		-it $(DOCKER_BUILD_TAG) zsh

DOCKER_RELEASE_TAG := cubao/release-examples
docker_build:
	docker build \
		--tag $(DOCKER_RELEASE_TAG) .
docker_push:
	docker push $(DOCKER_RELEASE_TAG)
docker_test_release:
	docker run --rm -v `pwd`:/workdir -it $(DOCKER_RELEASE_TAG) zsh
