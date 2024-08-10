## Notes 
ImGui won't add for some reason
but otherwise, should build

Only works for windows 11 currently i think but plans to make it work with linux

It's now in a good enough state. Still many things that aren't finished and it struggles with very large data sets. But otherwise it is fast enough  and can output data

# Silva

This is a model that aims to simulate the impact of forest fragmentation of the diversity and species distribtuion of ecosystems that experience Conspecific negative Density Dependence (CNDD). 
The model is based on that developed by May et al, (CONFETII, 2016, 2016 and 2020), with many alterrations to the competition and dispersal based on the Simulation model by Kalyuzny et al (2023). This model introduces the ability to simultainiously run several forest fragments simultainiuously in which indivisuals can immigrate between in a network. 

This was made as part of a masters project at the University of Sheffield by William George, supervised by Prof. robert Freckleton.

## Key Features
### Spatial Structure
Each fragment is modeled in a square space of length 𝐿 with a maximum carrying capacity 𝑁 (number of individuals).
This space can either be modelled as torus without boundries or wtih boundries where all offspring dispersed outside of the space is considered unsuccessful. 


### Time Steps
The simulation runs over discrete time steps. Each step involves:
1. Randomly selecting one individual (per fragment) to die.
2. Choosing a random parent to disperse a seed in a random direction based on a dispersal kernel.
    - The parent can also come from outside of the fragment through immigration
3. Competition from the neighbours surrounding the offspring will determine the success of the offspring
4. If successfull then the offspring is added to the simulation as an adult (recruitment), if unsuccessful a new parent is chosen until recruitment occurs

### Immigration
Seeds can be dispersed from outside the local community with a probability 𝑚, either from a large non-spatial metacommunity (when there is only one fragment) or another fragment based on connectivity.

### Fragments and Fragment Connectivity 
The total community can consist of several interconnected fragments. The interconectivity is set using a node map, and immigration between fragments (currently) is considered to be equally likley.

### Dispersal 
Each indivisual disperses using a 2Dt dispersal kernel:
```math
$$ 2Dt = \frac{b-1}{\pi a^2} \left(1 + \frac{x^2}{a^2}\right)^{-b} $$
```
Where x is the distance from the parent tree, b is a shape parameter (b = 2, for simplicity, Clark et al, 1999). a is set using the mean dispersal distance of trees from BCI:

```math
a = \mu \sqrt{\frac{2(b - \frac{3}{2})}{\pi}}
```

### Neighbourhood Interactions
At the point of dispersal, interactions between the potential recruit and surrounding adult individuals are calculated within a zone of influence (ZOI) with radius 𝑟 max. This is calculated using the Neighbourhood Competition Index (NCI): The probability that a potential recruit will establish is given by $\frac{1}{1 + \text{NCI}}$, where NCI is a function of the competition coefficient and the distance between the recruit and its neighbours:
```math
{NCI}_i = \sum_{j=0}^{n} \frac{c_{ij}}{1 + \left(\frac{d_{ij}}{b_1}\right)^{b_2}}
```


This program also uses a UI based on ImGui that allows for more quick iterations between different parametres.


NOTE: README is not finished 
