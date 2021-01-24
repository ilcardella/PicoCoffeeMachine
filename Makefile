ifeq ($(origin .RECIPEPREFIX), undefined)
  $(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or later)
endif
.RECIPEPREFIX = >

DOCKER_ENV_VARS = UID=$(shell id -u) GID=$(shell id -g)
DOCKER_BUILD = $(DOCKER_ENV_VARS) docker-compose -f docker/docker-compose.yml build --force-rm
DOCKER_RUN = $(DOCKER_ENV_VARS) docker-compose -f docker/docker-compose.yml run --rm
BUILD_SERVICE_NAME = pico-builder

default: build

docker:
> $(DOCKER_BUILD) $(BUILD_SERVICE_NAME)

build:
> $(DOCKER_RUN) $(BUILD_SERVICE_NAME) make local-build

local-build: clean
> mkdir build
> cd build && cmake .. && make

clean:
> rm -rf build

.PHONY: default docker build local-build clean
