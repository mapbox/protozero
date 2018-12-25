# Docker build of protozero

## Motivation

Check the protozero builds and tests for a variety of Linux flavours.

## User Guide

1. `docker build docker/ubuntu_18_04`
2. `docker image prune -af`

## Options

* REPO build from specified from git repository
* MIRROR use Ubuntu package mirror
  * Australia: `http://au.archive.ubuntu.com/ubuntu/`

For example:

`docker build docker/ubuntu_18_04 --build-arg MIRROR=http://au.archive.ubuntu.com/ubuntu/ `

## Notes

* Ubuntu 12.04 Precise does not support C++11 required for protozero
