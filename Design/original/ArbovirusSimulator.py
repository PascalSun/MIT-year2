#
# Simple prototype agent based arbovirus spread simulator.
#

import math
import sys
import json
import numpy as np

def main():

  ##################
  # Model Parameters

  params    = {} # see [1]

  ##################
  # Simulation State

  hosts     = {} # see [2]
  locations = {} # see [3]

  ################
  # Initialisation

  with open(sys.argv[1],'r') as pf:
    params = json.load(pf)

  # see [4]

  with open(params['locations-file'],'r') as f:
    locData = json.load(f)
  for cids in locData:
    cid            = int(cids)
    c              = locData[cids]
    c['cid']       = cid

    # build in function, a set of params
    c['hosts']     = set()
    locations[cid] = c


  with open(params['hosts-file'],'r') as f:
    hostData = json.load(f)
  for iids in hostData:
    iid        = int(iids)
    h          = hostData[iids]
    h['iid']   = iid
    hosts[iid] = h
    locations[h['location']]['hosts'].add(iid)

  ## add this to locations where the hosts is, iid means the id of the host


  #################
  # Simulation Loop

  cycle = 0
  while cycle < params['duration']:

    daytime = (cycle % 2) == 0 # even cycles are day

    infectionInVectors   (cycle,params,      locations)
    infectionInHosts     (cycle,params,hosts          )
    regularHumanMovement (cycle,params,hosts,locations)
    vectorMovement       (cycle,params,      locations)
    vectorInfection      (cycle,params,hosts,locations)
    hostInfection        (cycle,params,hosts,locations)
    vectorPopulation     (cycle,params,      locations)

  # HERE
  # seeding new infections (hosts)
  # human irregular movement

# TESTING
#    print "completed cycle " + str(cycle)
# TESTING
#    print hosts
#    print locations

    cycle += 1

##################################
# Process : regular human movement

def regularHumanMovement(cycle,params,hosts,locations):
  if isDaytime(cycle):
    for iid in hosts:
      # print iid
      h = hosts[iid]
      hub = h['hub']
      # If they have the hub
      if hub >= 0:
        moveHost(locations,h,hub)
  else:
    for iid in hosts:
      h = hosts[iid]
      moveHost(locations,h,h['home'])

def moveHost(locations,h,dst):
  iid           = h['iid']
  oldLoc        = h['location']
  h['location'] = dst
  locations[oldLoc]['hosts'].remove(iid)
  locations[dst]['hosts'].add(iid)

###########################
# Process : vector movement

def vectorMovement(cycle,params,locations):

  if not isDaytime(cycle): return # see [5]

  diffP = params['vector-diffusion-probability']

  # iterate over cells, accumulating incoming vectors
  # for each cell

  incoming = {}
  for cid in locations:
    c      = locations[cid]

    # choose numbers of outgoing and subtract

    movers = vectorMovementFrom(c,diffP)

    vSub(locations[cid]['v-state'],movers)

    # choose destinations for movers and accumulate

    dMovers = distributeVectors(c,movers)
    for nb in dMovers:
      if not incoming.has_key(nb):
        incoming[nb] = emptyVState()
      vAdd(incoming[nb],dMovers[nb])

  # all outgoing movement calculated, update state
  # by moving in all incoming vectors

  for cid in incoming:

    vAdd(locations[cid]['v-state'],incoming[cid])

def emptyVState(): return [0,0,0,0]

S = 0
E = 1
I = 2
R = 3
# Different stage of the viruse
# S Suptiitle
# E exposed
# I infectious
# R recovery
def vectorMovementFrom(c,diffP):
  vs = c['v-state']
  return map(lambda n : binSamp(n,diffP),vs)

def distributeVectors(c,movers):
  # print movers
  # print c
  nbs = c['neighbours']
  nnbs = len(nbs)
  dMovers = {}
  for i in range(0,nnbs):
    dMovers[nbs[i]] = emptyVState()
  for vs in [S,E,I]:
    for n in range(0,movers[vs]):
      nbt = np.random.randint(0,nnbs)
      dMovers[nbs[nbt]][vs] += 1
  return dMovers

#######################################
# Process : host to vector transmission

def vectorInfection(cycle,params,hosts,locations):
  br   = params['biting-rate'] # see [1]
  pbip = params['vector-infection-probability']
  for cid in locations:
    c  = locations[cid]
    ns = c['v-state'][S]
    if ns == 0: continue
    npop = 0
    ipop = 0
    # print c['hosts']
    for iid in c['hosts']:
      npop += 1
      if hosts[iid]['i-state'] == 'I': ipop += 1
    if ipop == 0: continue
    beta = (float(ipop) / float(npop)) * br * pbip # see [6]
    p    = 1.0 - math.exp(-beta)
    newInfections = binSamp(ns,p)
    c['v-state'][S] -= newInfections
    c['v-state'][E] += newInfections

#######################################
# Process : vector to host transmission

def hostInfection(cycle,params,hosts,locations):
  br   = params['biting-rate'] # see [1]
  pbip = params['host-infection-probability']
  for cid in locations:
    c  = locations[cid]
    nvi = c['v-state'][I]
    if nvi == 0: continue
    hpop = 0
    hs   = 0
    for iid in c['hosts']:
      if hosts[iid]['i-state'] == 'S': hs += 1
      hpop += 1
    if hs == 0: continue
    nv = sum(c['v-state'])
    # what's this for?
    beta = br*pbip*float(nvi) / float(hpop)
    p    = 1.0 - math.exp(-beta)
    for iid in c['hosts']:
      h = hosts[iid]
      if h['i-state'] != 'S': continue
      if binSamp(1,p):
        h['i-state'] = 'E'
        print csvLine(infectionEvent(cycle,h,c)) # see [8]

def infectionEvent(cycle,h,c):
  return [cycle,'i',h['iid'],c['cid']]


############################################
# Process : infection progression in vectors
# Exposed to Infectious for verctor
def infectionInVectors(cycle,params,locations):

  eToIRate = params['vector-e-to-i-rate'] # see [1]
  pTrans   = 1.0 - math.exp(-eToIRate)

  for cid in locations:
    c = locations[cid]
    ne = c['v-state'][E]

    if ne == 0: continue
    nTrans = binSamp(ne,pTrans)
    c['v-state'][E] -= nTrans
    c['v-state'][I] += nTrans

############################################
# Process : infection progression in hosts

def infectionInHosts(cycle,params,hosts):

  eToIRate = params['host-e-to-i-rate'] # see [1]
  iToRRate = params['host-i-to-r-rate'] # see [1]
  pITrans   = 1.0 - math.exp(-eToIRate)
  pRTrans   = 1.0 - math.exp(-iToRRate)

  for iid in hosts:
    h = hosts[iid]
    s = h['i-state']
    if s == 'E':
      if binSamp(1,pITrans): h['i-state'] = 'I'
    elif s == 'I':
      if binSamp(1,pRTrans): h['i-state'] = 'R'

############################################
# Process : vector population dynamics

def vectorPopulation(cycle,params,locations):

  br0 = params['vector-birth-rate']        # see [1]
  mr  = params['vector-maturation-rate']   # see [1]
  dr  = params['vector-death-rate']        # see [1]
  pDeath = 1.0 - math.exp(-dr)
  pMat   = 1.0 - math.exp(-mr)

  for cid in locations:
    c = locations[cid]
    vs = c['v-state']
    cc = c['v-capacity']
    nDeaths = map(lambda n : binSamp(n,pDeath),vs)

    vPop    = sum(vs)
    br      = float(vPop) * br0 * max(0.0,1.0 - vPop/cc)
    nBirths = 0
    if br >= 0.0: nBirths = np.random.poisson(br)

    nMats   = binSamp(c['v-immature'],pMat)

    vSub(vs,nDeaths)
    c['v-immature'] += nBirths
    c['v-immature'] -= nMats
    vs[S]           += nMats

#######
# Tools

def isDaytime(cycle):
  return (cycle % 2) == 0 # even cycles are day

######
# Misc

# because of quirk in np.random.binomial
def binSamp(n,p):
  if n == 0: return 0
  return np.random.binomial(n,p)

def vSum(u,v):
  r = []
  for i in range(0,len(u)): r.append(u[i] + v[i])
  return r

# modifies first arg ie "to u add v"
def vAdd(u,v):
  for i in range(0,len(u)): u[i] += v[i]

# modifies first arg ie "from u subtract v"
def vSub(u,v):
  for i in range(0,len(u)): u[i] -= v[i]

def csvLine(parts):
  return (','.join((str(x) for x in parts)))

######
# Main

main()

#######
# Notes

'''
  [1] Model parameters required or allowed are as follows:

  'hosts-file'     :
  'locations-file' :
  'duration'       : number of cycles to run simulation
  'vector-diffusion-probability' :
    per (active) cycle probability of a vector moving to an
    adjacent cell; this would be calculated from eg a diffusion
    coefficient and cell geometry
  'biting-rate'    : mean number of bites per cycle for each
                     vector
  'vector-infection-probability' :
  'host-infection-probability'   :
    probability of infection per bite given susceptible
    vector (host) and infectious host (vector)
  'vector-e-to-i-rate' :
  'host-e-to-i-rate'   :
  'host-i-to-r-rate'   :
    per-cycle mean rates of infection progression (i.e.
    mean period is 1/rate in cycles)
  'vector-birth-rate' : per-cycle mean number of immature
    vectors created by each adult vector
  'vector-maturation-rate' : per-cycle mean rate of immature
    vectors becoming adults
  'vector-death-rate' : per-cycle vector mortality (i.e.
    mean lifespan is 1/rate)

  [2] Hosts are keyed on an individual ID integer (IID). Host
      objects are dictionaries with the following fields

  'i-state'  : infection state, character S,E,I or R
  'location' : location ID of current cell

  fixed (not dynamic state):

  'iid'      : host id
  'home'     : location ID of home cell
  'hub'      : location ID of daytime cell (-1 for none)

  The serialised form is a JSON dictionary, keyed on string
  representations of host ID numbers.

  [3] Locations are keyed on cell ID integers (CID). Location
      objects are dictionaries with:

  'hosts'       : set of IIDs of hosts currently in cell
  'v-capacity'  : density (vectors per cell) at
    which birth rate becomes zero
  'v-immature'  : number of immature (egg, larvae, pupae) in cell
  'v-state'     : stored as a 4-vector or numbers in states
                  SEIR. A bit brittle if number of states changes
                  eg locations[27]['v-state'][I] is the number
                  of vectors currently in I state in cell 27.
                  R is not actually used for vectors.

  fixed (not dynamic state):

  'cid'         : cell id
  'coordinates' : list of float co-ordinates ie [x,y]

  The serialised form is a JSON dictionary, keyed on string
  representations of location ID numbers.

  [4] This is where parameter checking would go.

  [5] Assume vectors are diurnal movers.

  [6] Derivation of force of infection on vectors.

  We want to calculate the expected number of infectious
  bites for each vector.

  br :
    number of bites by vector
  br * (ih/nh) :
    only (ih/nh) of bites will be on infectious hosts
  br * (ih/in) * pbip :
    only some of those bites will transmit

  beta = (float(ipop) / float(npop)) * br * pbip

  [7] Derivation of force of infection on hosts.

  We want the expected number of infectious bites on each host.

  br :
    number of bites per vector
  br * iv :
    total number of bites by infectious vectors
  br * iv * (1/nh) :
    number of those bites on each human
  br * iv * (1/nh) * pbip :
    only some of those bites will transmit

    beta = br*pbip*float(nvi) / float(hpop)

  [8] Events are written to stdout as CSV lines. The first two
      fields are always the cycle number and an event type
      string. The remaining fields depend on the event type.

      Event types:
      'i' : host infection event
            field 0, 1 : cycle, 'i'
            field 2, 3 : iid, cid

'''
