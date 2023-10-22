## Data Proxy

The `data-proxy` folder contains a simple go-webserver that is hosted on https://app-side.net:10000.
It is used by the Arduino to publish its data. It is managed by a Linux service. 

It requires encrypted certificate and privkey to exist in the hard-coded locations. The "key variable" is hard-coded into the compiled binary on the server.
