from simpleai.search import SearchProblem
# from simpleai.search import breadth_first,depth_first,astar,greedy
import simpleai.search

class GameProblem(SearchProblem):
    MAP=None
    POSITIONS=None
    INITIAL_STATE=None
    GOAL=None
    CONFIG=None
    AGENT_START=None
    SHOPS=None
    CUSTOMERS=None
    MAXBAGS = 0
    MOVES = ('West','North','East','South')
    customer3 = 0
    customer2 = 0
    customer1 = 0
    customer22 = 0
    customer12 = 0
    created = 0

   # --------------- Common functions to a SearchProblem -----------------

    def actions(self, state):

        #List of possible actions
        actions=[]

        #Variables of different states
        position = state[0]

        #Next positions in every direction
        North = (position[0], position[1]-1)
        South = (position[0], position[1]+1)
        West = (position[0]-1, position[1])
        East = (position[0]+1, position[1])

        #Add a movement action only if the next position is in a possible square such as a street, a customer or a pizzeria (always inside the borders)
       # if North in self.POSITIONS['street'] or North in self.SHOPS or North in self.CUSTOMERS or North in self.POSITIONS['start']:
        if North not in self.POSITIONS['building']:  
            actions.append('North')
       # if South in self.POSITIONS['street'] or South in self.SHOPS or South in self.CUSTOMERS or South in self.POSITIONS['start']:
        if South not in self.POSITIONS['building']: 
            actions.append('South')
       # if West in self.POSITIONS['street'] or West in self.SHOPS or West in self.CUSTOMERS or West in self.POSITIONS['start']:
        if West not in self.POSITIONS['building']:
            actions.append('West')
      #  if East in self.POSITIONS['street'] or East in self.SHOPS or East in self.CUSTOMERS or East in self.POSITIONS['start']:
        if East not in self.POSITIONS['building']:
            actions.append('East')

        #Add Load only if on a pizzeria
        if position in self.POSITIONS['pizza'] :
            actions.append('Load')
            #Add also movement in order to get put of the pizzeria
            if North in self.POSITIONS['street'] or North in self.SHOPS or North in self.CUSTOMERS or North in self.POSITIONS['start']:
                actions.append('North')
            if South in self.POSITIONS['street'] or South in self.SHOPS or South in self.CUSTOMERS or South in self.POSITIONS['start']:
                actions.append('South')
            if West in self.POSITIONS['street'] or West in self.SHOPS or West in self.CUSTOMERS or West in self.POSITIONS['start']:
                actions.append('West')
            if East in self.POSITIONS['street'] or East in self.SHOPS or East in self.CUSTOMERS or East in self.POSITIONS['start']:
                actions.append('East')

        #Add Deliver only if in a house (checking previously if that kind of customer exists)
        if self.customer3==1:
            if state[0] in state[5]:
                actions.append('Deliver')
        if self.customer2==1:
            if state[0] in  state[4]:
                actions.append('Deliver')
        if self.customer1==1:
            if state[0] in state[3]:
                actions.append('Deliver')

        return actions

    def result(self, state, action):

        #Default value
        next_state = (state[0], state[1], state[2], state[3], state[4], state[5])

        #If on a pizzeria pick up bags
        if state[0] in self.POSITIONS['pizza'] and action == 'Load':
            if state[2] > self.CONFIG['maxBags']: #If there are still more than we can carry pick up the maximum
                next_state = (state[0], self.CONFIG['maxBags'], state[2], state[3], state[4], state[5])
            elif state[2] <= self.CONFIG['maxBags'] and state[2] != state[1]:
                next_state = (state[0], state[2], state[2], state[3], state[4], state[5]) #If there are less customers that we can carry pick up the exact amount


        elif action == 'Deliver':
            #If on a house deliver and remove customer from tuple
            if self.customer3==1:
                if state[0] in state[5] and state[1]>=3 and action == 'Deliver':
                    next_state = (state[0], state[1]-3, state[2]-3, state[3], state[4], state[5][:state[5].index(state[0])] + state[5][state[5].index(state[0])+1:])
                #If the customer is going to need one more pizza add it to that list
                elif state[0] in state[5] and state[1]==2 and action == 'Deliver':
                    self.customer12=1
                    next_state = (state[0], state[1]-2, state[2]-2,state[3] +(state[0],), state[4], state[5][:state[5].index(state[0])] + state[5][state[5].index(state[0])+1:])
                #Same with two more
                elif state[0] in state[5]  and state[1]==1 and action == 'Deliver':
                    self.customer22=1
                    next_state = (state[0], state[1]-1, state[2]-1, state[3], state[4] + (state[0],), state[5][:state[5].index(state[0])] + state[5][state[5].index(state[0])+1:])
            if self.customer22==1:
                if state[0] in state[4]  and state[1]>=2 and action == 'Deliver':
                    next_state = (state[0], state[1]-2, state[2]-2, state[3], state[4][:state[4].index(state[0])] + state[4][state[4].index(state[0])+1:], state[5])
                elif state[0] in state[4]  and state[1]==1 and action == 'Deliver':
                    self.customer12=1
                    next_state = (state[0], state[1]-1, state[2]-1, state[3] + (state[0],), state[4][:state[4].index(state[0])] + state[4][state[4].index(state[0])+1:], state[5])

            if self.customer12==1:
                if state[0] in state[3]  and state[1]>=1 and action == 'Deliver':
                    next_state = (state[0], state[1]-1, state[2]-1, state[3][:state[3].index(state[0])] + state[3][state[3].index(state[0])+1:], state[4], state[5])
  #If action is move and we can move, then move
        elif action == 'South' and ((state[0][0], state[0][1]+1) not in self.POSITIONS ['building']) and state[0][1]+1<self.CONFIG['map_size'][1]:
            next_state = ((state[0][0], state[0][1]+1), state[1], state[2], state[3], state[4], state[5])
        elif action == 'North' and ((state[0][0], state[0][1]-1) not in self.POSITIONS ['building']) and state[0][1]-1>=0:
            next_state = ((state[0][0], state[0][1]-1), state[1], state[2], state[3], state[4], state[5])
        elif action == 'East' and ((state[0][0]+1, state[0][1]) not in self.POSITIONS ['building']) and state[0][0]+1<self.CONFIG['map_size'][0]:
            next_state = ((state[0][0]+1, state[0][1]), state[1], state[2], state[3], state[4], state[5])
        elif action == 'West' and ((state[0][0]-1, state[0][1]) not in self.POSITIONS ['building']) and state[0][0]-1>=0:
            next_state = ((state[0][0]-1, state[0][1]) , state[1], state[2], state[3], state[4], state[5])
        return next_state

    def is_goal(self, state):
        return state == self.GOAL

    def cost(self, state, action, state2):
        cost = 0
        #One cost per movement
        if action in ['North', 'South', 'West', 'East', 'Load', 'Deliver']:
            cost = 1
        return cost

    def heuristic(self, state):
        #Manhattan heuristic
        return abs(state[0][0] - self.GOAL[0][0]) + abs(state[0][1] - self.GOAL[0][1])

    #Initialize everything in the problem
    def setup (self):
        print('\nMAP: ', self.MAP, '\n')
        print('POSITIONS: ', self.POSITIONS, '\n')
        print('CONFIG: ', self.CONFIG, '\n')

        #Check if customer3 exists
        try:
            self.POSITIONS['customer3']
        except:
            self.customer3 = 0
        else:
            self.customer3 = 1

        #Check if customer2 exists
        try:
            self.POSITIONS['customer2']
        except:
            self.customer2 = 0
        else:
            self.customer2 = 1
            self.customer22=1

        #Check if customer1 exists
        try:
            self.POSITIONS['customer1']
        except:
            self.customer1 = 0
        else:
            self.customer1 = 1
            self.customer12=1
        
        initial_state=(self.POSITIONS['start'][0], 0, 0, (), (), ())

        if self.customer3==1:
            #Three pizzas per customer3
            self.CUSTOMERS=self.POSITIONS['customer3']
            initial_state =(initial_state[0], initial_state[1], initial_state[2] + len(self.POSITIONS['customer3'])*3, initial_state[3], initial_state[4], tuple(self.POSITIONS['customer3']))
            self.created = 1 #The created flag is used to either initialize or add a value to both deliver and self.CUSTOMERS

        if self.customer2==1 and self.created==0:
            #Two pizzas per customer2
            self.CUSTOMERS=self.POSITIONS['customer2']
            initial_state =(initial_state[0], initial_state[1], initial_state[2] + len(self.POSITIONS['customer2'])*2, initial_state[3], tuple(self.POSITIONS['customer2']), initial_state[5])
            self.created = 1
        elif self.customer2==1 and self.created==1:
            self.CUSTOMERS.append(self.POSITIONS['customer2'])
            initial_state =(initial_state[0], initial_state[1], initial_state[2] + len(self.POSITIONS['customer2'])*2, initial_state[3], tuple(self.POSITIONS['customer2']), initial_state[5])

        if self.customer1==1 and self.created==0:
            #One pizza per customer1
            self.CUSTOMERS=self.POSITIONS['customer1']
            initial_state =(initial_state[0], initial_state[1], initial_state[2] + len(self.POSITIONS['customer1']), tuple(self.POSITIONS['customer1']), initial_state[4], initial_state[5])
        elif self.customer1==1 and self.created==1:
            self.CUSTOMERS.append(self.POSITIONS['customer1'])
            initial_state =(initial_state[0], initial_state[1], initial_state[2] + len(self.POSITIONS['customer1']), tuple(self.POSITIONS['customer1']), initial_state[4], initial_state[5])

        final_state= (self.POSITIONS['start'][0], 0, 0, (), (), ()) #Final state should be the same as the initial state but with every pizza delivered
        algorithm= simpleai.search.astar
        self.SHOPS=self.POSITIONS['pizza']
        return initial_state,final_state,algorithm

    def printState (self,state):

        #Print every information inside state in a clean way
        return '\n\n ----- STATE -----\n\nPosition: ' + str(state[0]) + '\nBags: ' + str(state[1]) + '\nRemaining deliveries: ' + str(state[2])

    def getPendingRequests (self,state):

        #Default value
        pending = None

        #If there are customer3 and we are in one return 3
        if state[0] in state[5]:
                pending = 3

        #If there are customer2 and we are in one return 2
        elif state[0] in state[4]:
                pending = 2

        #If there are customer1 and we are in one return 1
        elif state[0] in state[3]:
                pending = 1

        #If on a satisfied customer return 0
        else:
            if state[0] not in self.POSITIONS['street'] and state[0] not in self.POSITIONS['pizza'] and state[0] not in self.POSITIONS['start']:
                pending = 0
        
        return pending

    # -------------------------------------------------------------- #
    # --------------- DO NOT EDIT BELOW THIS LINE  ----------------- #
    # -------------------------------------------------------------- #

    def getAttribute (self, position, attributeName):
        '''Returns an attribute value for a given position of the map
           position is a tuple (x,y)
           attributeName is a string

           Returns:
               None if the attribute does not exist
               Value of the attribute otherwise
        '''
        tileAttributes=self.MAP[position[0]][position[1]][2]
        if attributeName in tileAttributes.keys():
            return tileAttributes[attributeName]
        else:
            return None

    def getStateData (self,state):
        stateData={}
        pendingItems=self.getPendingRequests(state)
        if pendingItems >= 0:
            stateData['newType']='customer{}'.format(pendingItems)
        return stateData

    # THIS INITIALIZATION FUNCTION HAS TO BE CALLED BEFORE THE SEARCH
    def initializeProblem(self,map,positions,conf,aiBaseName):
        self.MAP=map
        self.POSITIONS=positions
        self.CONFIG=conf
        self.AGENT_START = tuple(conf['agent']['start'])

        initial_state,final_state,algorithm = self.setup()
        if initial_state == False:
            print ('-- INITIALIZATION FAILED')
            return True

        self.INITIAL_STATE=initial_state
        self.GOAL=final_state
        self.ALGORITHM=algorithm
        super(GameProblem,self).__init__(self.INITIAL_STATE)

        print ('-- INITIALIZATION OK')
        return True

    # END initializeProblem

