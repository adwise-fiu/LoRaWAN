## Acknowledgments ##

The initial version of this code was developed by SIGNET Lab at
the University of Padova by the developer Davide Magrin and one of the contributors Qiu Yukang [LoRaWAN](https://github.com/signetlabdei/lorawan "LoRaWAN GitHub") [LoRaWAN-Class-C](https://github.com/QiuYukang/lorawan "LoRaWAN Class C")

# LoRaWAN ns-3 modified module #

This is an [ns-3](https://www.nsnam.org "ns-3 Website") module that can be used
to perform simulations of a [LoRaWAN](https://lora-alliance.org/about-lorawan "LoRa Alliance") network.



[Module Documentation](https://signetlabdei.github.io/lorawan-docs/models/build/html/lorawan.html).

## Getting started ##

### Prerequisites ###

First of all in order to run the simulations using this module, you need to install ns-3, and clone this repository inside the `src` directory:

```bash
git clone https://github.com/nsnam/ns-3-dev-git ns-3
git clone https://github.com/adwise-fiu/LoRaWAN ns-3/src/lorawan
```

### Compilation ###

To compile, move to the `ns-3` folder, configure and then build ns-3:

```bash
./waf configure --enable-tests --enable-examples
./waf build
```

Finally, make sure tests run smoothly with:

```bash
./test.py -s lorawan
```

Make sure the script returns that the lorawan test suite passed.

## Main Code ##

The module includes the following code:

- `LoraCenter`

The code can be run via the `./waf --run LoraCenter` command. 

The Simulation Setup is shown in the figure below:
![alt text](https://github.com/adwise-fiu/LoRaWAN/blob/main/Figure/Final%20Approach%20Setup.png)


## Documentation ##

- [ns-3 tutorial](https://www.nsnam.org/docs/tutorial/html "ns-3 Tutorial")
- [ns-3 manual](https://www.nsnam.org/docs/manual/html "ns-3 Manual")
- The LoRaWAN specification can be requested at the [LoRa Alliance
  website](http://www.lora-alliance.org)
  
