FROM node:10.15.1

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY . /usr/src/app

RUN yarn global add esy
RUN esy install
RUN esy build

EXPOSE 3000

CMD esy x SummarizeApp.exe
