module Agent;

// Agents have lots of things going on in them. Like, they totally have charisma
// and intelligence. And a level of interest in talking about stuff. And stuff.
// Hmm, this is gonna be tricky.
class Agent
{ 
    // Charisma is a one-sided buff to trying to convince his partner of a thing
    // where a higher charisma means less talkingPoints are required to push
    // the idea
    int charisma;
    // Intelligence is a two-sided check. People of equal intelligence basically
    // make an intelligence check not occur. If the talker has a higher
    // intelligence than the partner, he gets a boost. If the talker has a much
    // lower intelligence, he gets a penalty, with the unique extra addition
    // that if the talker has a lower intelligence and a much higher charisma,
    // he gets an added debuff (corrupt politician syndrome)
    int intelligence;
    // This is excellently named, if misleadingly. This accounts for how much
    // effort it takes to actually inflict a thought change in his 
    // conversational partner, as in, takes more talkingPoints to change the
    // mind of a staunch opponent of his than it does to improve for someone
    // neutral to the topic. This is the maximum value of talking points the
    // agent can have at one time
    int talkingPoints;
    // This is the current number of talkingPoints the agent has
    int curTalkPoints;
    // This is the recharge rate of the agent, where 1 means 1 talking point
    // a simulation tick
    int tpRecharge;
    // These are all the ideas that anyone in the entire world can have. Some
    // are dichtomous, others are mutually independent. The dichotomous ones
    // simply mean that For every point stored in one, the opposite idea has
    // one less point, for a maximum shift of 100 in one idea, -100 in another.
    // (Democrat vs. Republican)
    // Furthermore, the mutually exclusive ideas don't seem to have a natural
    // reason to do anything besides grow until at maximum, so we shall see if
    // I can't come up with an antagonist to unchecked growth for independent
    // ideas
    Idea[] ideas;
    // This is the basic constructor for an agent, leading to a purely random
    // agent. Use another constructor for deliberately initializing values
    Agent()
    {
        randomizeAgent();
    }
    // This is the initializer for a random population. Randomly chooses a
    // charisma and intelligence and maximum talking points value, as well as
    // the talking points recharge factor.
    // Also initializes a random number of ideas to a random value. Use this
    // fucntion for a purely random agent
    void randomizeAgent()
    {
        return;
    }
    // This is a one sided exchange, where this agent spends points and does
    // checks to influence the agent he is speaking with
    void oneSidedConvo( Agent other )
    {
        return;
    }
    // Might not use this, but here for completeness of idea, this is as above
    // except it is a two sided conversation. One agent is the starter, but then
    // after this agent does their check, it forces a call to the other agents
    // one sided convo function, to emulate a two sided conversation
    void twoSidedConvo( Agent other )
    {
        return;
    }
}
