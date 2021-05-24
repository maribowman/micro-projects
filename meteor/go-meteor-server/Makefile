NAME		= marschine/go-meteor-server
GIT_BRANCH	= $(shell git rev-parse --abbrev-ref HEAD)
GIT_HASH	= $(shell git rev-parse --short HEAD)
TAG			= $(GIT_BRANCH)_$(GIT_HASH)
IMAGE		= $(NAME):$(TAG)
STAGE		= local

### docker
build:
	docker build -q -t $(IMAGE) -t $(NAME):latest .
	docker image prune -f --filter label=stage=builder

run: build
	docker run -d --rm -p 4040:4040 --name $(TAG) $(IMAGE) $(STAGE)

push: build
	docker push $(IMAGE)
	docker push $(NAME):latest
