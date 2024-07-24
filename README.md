## Notes
ImGui won't add for some reason
but otherwise, should build
This is my first project wiht c++ so it won't be great. sorry
Also the UI and simulation arent put together yet so That won't run, just getting it out there

# Silva

This is a model that aims to simulate the impact of forest fragmentation of the diversity and species distribtuion of ecosystems that experience Conspecific negative Density Dependence (CNDD). 
The model is based on that developed by May et al, (CONFETII, 2016, 2016 and 2020), with many alterrations to the competition and dispersal based on the Simulation model by Kalyuzny et al (2023). This model introduces the ability to simultainiously run several forest fragments simultainiuously in which indivisuals can immigrate between in a network. 

This was made as part of a masters project at the University of Sheffield by William George, supervised by Prof. robert Freckleton.

## Key Features
### Spatial Structure
Each fragment is modeled in a square space of length 𝐿 with a maximum carrying capacity 𝑁 (number of individuals).

### Fragmentation: 
The total community can consist of several interconnected fragments. In continuous forests, the space is modeled as a torus (no boundaries). In fragmented forests, any offspring dispersed outside the fragment is considered unsuccessful.

### Time Steps
The simulation runs over discrete time steps. Each step involves:
1. Randomly selecting one individual (per fragment) to die.
2. Choosing a random parent to disperse a seed in a random direction based on a dispersal kernel.

### Immigration

Seeds can also be dispersed from outside the local community with a probability 𝑚, either from a large non-spatial metacommunity (when there is only one fragment) or another fragment.

### Neighbourhood Interactions

At the point of dispersal, interactions between the potential recruit and surrounding adult individuals are calculated within a zone of influence (ZOI) with radius 𝑟 max. This is calculated using the Neighbourhood Competition Index (NCI): The probability that a potential recruit will establish is given by $\frac{1}{1 + \text{NCI}}$, where NCI is a function of the competition coefficient and the distance between the recruit and its neighbours.


This program also uses a UI based on ImGui that allows for more quick iterations between different parametres.


NOTE: README is not finished 
