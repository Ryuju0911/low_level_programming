IMAGE_NAME=low_level_programming
CONTAINER_NAME=low_level_programming
FORCE_RM=true
SHM_SIZE=2g

stop:
	docker stop $(CONTAINER_NAME)

build:
	docker build \
		-f ./Dockerfile \
		-t $(IMAGE_NAME) \
		--no-cache \
		--force-rm=$(FORCE_RM) \
		--platform=linux/amd64 \
		.

run:
	docker run \
		-dit \
		-v $$(PWD):/workspace \
		--name $(CONTAINER_NAME) \
		--privileged \
		--rm \
		--shm-size $(SHM_SIZE) \
		$(IMAGE_NAME)

exec:
	docker exec \
	  -it \
  	$(CONTAINER_NAME) bash
