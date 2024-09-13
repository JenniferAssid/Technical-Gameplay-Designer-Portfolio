/*******************************************************************************
Author:    Benjamin Ellinger
DP Email:  bellinge@digipen.edu
Date:      2/1/2022
Course:    DES 212

Description:
	This file is part of the framework for the 1D Combat Simulator assignment in
	DES 212 (System Design Methods). It can be freely used and modified by students
	for that assignment.
	
    This component controls the entire combat simulation. This component is added to
    a game object whose only purpose to contain this functionality, but using a
    ScriptedObject would potentially be a more advanced	way of doing this.
	
*******************************************************************************/

/*******************************************************************************
Author:    Jennifer Assid
Date:      Spring 2022
Course:    DES 212

Description:
	I have adjusted this template to exhibit unique behaviors. I've highlighted
    big sections of changes below with comments.
	
*******************************************************************************/

//Standard Unity component libraries
using System.Collections; //Not needed in this file, but here just in case.
using System.Collections.Generic; //Not needed in this file, but here just in case.
using System.IO; //Needed for writing telemetry data to a file.
using UnityEngine; //The library that lets your access all of the Unity functionality.
using UnityEngine.UI; //This is here so we don't have to type out longer names for UI components.

//Inherits from MonoBehavior like all normal Unity components do...
//Remember that the class name MUST be identical to the file name!
public class SimControl : MonoBehaviour
{
    //Does the simulation start in Auto mode?
    public static bool AutoMode = false;
    //Does the simulation start in Fast mode?
    public static bool FastMode = false;

    public static bool GroupMode = false;
    public static bool MultiGroupMode = false;
    //This is the delta time the simulation uses,
    //which is artificially increased when in fast mode.
    public static float DT;

    //How many different AI types (and therefore how many "fights") do we want?
    public int Fights = 8;
    private int FightCount = 0;
    public static bool SimOver = false; //Have all the fights been completed?
    public static string CurrentAI = "Random"; //What's the current type of AI for this fight?

    //How many rounds is each "fight"?
    public int Rounds = 4;
    private int RoundCount = 0;
    public static bool RoundOver = false; //Did the current round just end?
    public static bool RoundStart = false; //Is a new round just starting (make sure the player has time to find a target)?
    //How long a delay between rounds?
    public float RoundDelay = 3.0f;
    private float RoundTimer = 3.0f;

    //How far from the center of the screen is the "edge" of the arena?
    public static float EdgeDistance = 8.0f;
    //How far from the center of the screen do combatants start?
    public static float StartingX = 6.0f;
    public static float StartingY = 1.5f;

    public static int AbilityOneUseCount = 0;
    public static int AbilityTwoUseCount = 0;
    public static int AbilityThreeUseCount = 0;
    public static int AbilityFourUseCount = 0;
    public static int AbilityFiveUseCount = 0;

    public static int[] EnemyDestroyed;

    public static int[] EnemyDeployed;

    public static int sub_RoundCount = 0;
    public int sub_RoundMax = 10;
    public static int sub_FightCount = 0;

    //Telemetry data for an individual fight.
    public static int Victories = 0;
    public static int Defeats = 0;
    public static float DamageDone = 0;
    public static float TotalFightTime = 0;
    public static float TotalRoundTime = 0;
    public static float MaxRoundTime = 20.0f;
    public static StreamWriter DataStream; //Stream used to write the data to a file.

    //Need a reference to the player, so we don't have to look it
    //up each time.
    public static Hero Player;

    //We will use the UI canvas a lot, so store a reference to it.
    public static GameObject Canvas;

	//References for text prefabs and enemy prefabs, so we don't
	//have to load them each time.
    public static GameObject InfoTextPrefab;
    public static GameObject StaticInfoTextPrefab;
    public static GameObject EnemyType1Prefab; //Should really be an array or dictionary.
    public static GameObject EnemyType2Prefab;
    public static GameObject EnemyType3Prefab;
    public static GameObject EnemyType4Prefab;
    public static GameObject EnemyType5Prefab;
    public static GameObject EnemyType6Prefab;

    public static int FightType = 0;

    public static UpdateText RoundText;

    //Start is called before the first frame update
    void Start()
    {
        //Create a comma-separated value file to output telemetry data.
        //This can just then be directly opened in Excel.
        DataStream = new StreamWriter("FightData.csv", false);
        // ADJUSTED TELEMETRY TO FIT SPECIFICS - Jennifer Assid
        DataStream.WriteLine("AI TYPE,VICTORIES,DEFEATS,DPS,FIGHT LENGTH, ABILITY ONE, ABILITY TWO, ABILITY THREE, ABILITY FOUR, ABILITY FIVE, " +
            "GRUNT DESTROYED, GRUNT DEPLOYED, MELEE DESTROYED, MELEE DEPLOYED, RANGED DESTROYED, RANGED DEPLOYED, TANK DESTROYED, TANK DEPLOYED, HEALER DESTROYED, HEALER DEPLOYED, ELITE DESTROYED, ELITE DEPLOYED, MULTIGROUP"); //Write some headers for our columns.

        //Get a reference to the canvas (used for UI objects).
        Canvas = GameObject.Find("Canvas");

        //Get a reference to the player's game object.
        //Note that we use GetComponent so we don't have to do that
        //every time we want to access the Hero class functionality.
        Player = GameObject.Find("Hero").GetComponent<Hero>();

        //Load all the prefabs we are going to use.
        InfoTextPrefab = Resources.Load("Prefabs/InfoText") as GameObject;
        StaticInfoTextPrefab = Resources.Load("Prefabs/StaticInfoText") as GameObject;
        EnemyType1Prefab = Resources.Load("Prefabs/GruntEnemy") as GameObject;
        EnemyType2Prefab = Resources.Load("Prefabs/MeleeEnemy") as GameObject;
        EnemyType3Prefab = Resources.Load("Prefabs/SniperEnemy") as GameObject;
        EnemyType4Prefab = Resources.Load("Prefabs/TankEnemy") as GameObject;
        EnemyType5Prefab = Resources.Load("Prefabs/HealerEnemy") as GameObject;
        EnemyType6Prefab = Resources.Load("Prefabs/EliteEnemy") as GameObject;

        EnemyDestroyed = new int[6];
        EnemyDeployed = new int[6];

        RoundText = GameObject.Find("RoundCount").GetComponent<UpdateText>();
    }

    //Update is called once per frame
    void Update()
    {
        //If the ESC key is pressed, exit the program.
        if (Input.GetKeyDown(KeyCode.Escape) == true)
            Application.Quit();

        //The simulation is over, so stop updating.
        if (FightCount >= Fights)
        {
            if (SimOver == false) //Did the simulation just end?
            {
                SimOver = true;
                DataStream.Close(); //Don't forget to close the stream.
                SpawnInfoText("SIMULATION OVER", true);
            }
            return;
        }

		// CREATED CUSTOM FUNCTIONALITY FOR STATE CHANGES - Jennifer Assid
        if (Input.GetKeyDown(KeyCode.A) == true)
        {
            if (FastMode) return;
            AutoMode = !AutoMode;

            EnabledText enabledText = GameObject.Find("Auto Mode").GetComponent<EnabledText>();

            if (AutoMode)
                enabledText.isEnabled = true;
            else
                enabledText.isEnabled = false;
        }
        // CREATED CUSTOM FUNCTIONALITY FOR STATE CHANGES - Jennifer Assid
        if (Input.GetKeyDown(KeyCode.F) == true)
        {
            if (FastMode == false)
            {
                GroupMode = false;
                MultiGroupMode = false;
                FightCount = 0;
                CurrentAI = "Random";
                RoundCount = 0;
                AbilityFiveUseCount = 0;
                AbilityFourUseCount = 0;
                AbilityThreeUseCount = 0;
                AbilityTwoUseCount = 0;
                AbilityOneUseCount = 0;
                for (int i = 0; i < 6; i++)
                {
                    EnemyDeployed[i] = EnemyDestroyed[i] = 0;
                }
                sub_FightCount = 0;
                sub_RoundCount = 0;
                Victories = 0;
                Defeats = 0;
                DamageDone = 0;
                TotalFightTime = 0;
                TotalRoundTime = 0;
                RoundText.SetMainNumber(FightCount);
                RoundText.SetSecondaryNumber(RoundCount);
                RoundText.SetTertiaryNumber(sub_RoundCount);
            }

            FastMode = !FastMode;
            EnabledText enabledText = GameObject.Find("Fast Mode").GetComponent<EnabledText>();

            if (FastMode)
            {
                enabledText.isEnabled = true;
                AutoMode = true;
                GroupMode = false;
            }
            else
            {
                enabledText.isEnabled = false;
                AutoMode = false;
                GroupMode = false;
            }

            RoundCount = 0;
            FightCount = 0;
            NewRound();
        }
        // CREATED CUSTOM FUNCTIONALITY FOR STATE CHANGES - Jennifer Assid
        if (Input.GetKeyDown(KeyCode.G) == true)
        {
            GroupMode = !GroupMode;
            EnabledText enabledText = GameObject.Find("Group Mode").GetComponent<EnabledText>();

            if (GroupMode)
            {
                enabledText.isEnabled = true;
                MultiGroupMode = false;
                GameObject.Find("MultiGroup Mode").GetComponent<EnabledText>().isEnabled = false;
            }
            else
                enabledText.isEnabled = false;

            RoundCount--;
            NewRound();
        }

        if (Input.GetKeyDown(KeyCode.M) == true)
        {
            MultiGroupMode = !MultiGroupMode;
            EnabledText enabledText = GameObject.Find("MultiGroup Mode").GetComponent<EnabledText>();

            if (MultiGroupMode)
            {
                enabledText.isEnabled = true;
                GroupMode = false;
                GameObject.Find("Group Mode").GetComponent<EnabledText>().isEnabled = false;
            }
            else
                enabledText.isEnabled = false;

            RoundCount--;
            NewRound();
        }
        // CREATED CUSTOM FUNCTIONALITY FOR STATE CHANGES - Jennifer Assid
        if (Input.GetKeyDown(KeyCode.R) == true)
        {
            FightCount = 0;
            CurrentAI = "Random";
            RoundCount = 0;
            AbilityFiveUseCount = 0;
            AbilityFourUseCount = 0;
            AbilityThreeUseCount = 0;
            AbilityTwoUseCount = 0;
            AbilityOneUseCount = 0;
            for (int i = 0; i < 6; i++)
            {
                EnemyDeployed[i] = EnemyDestroyed[i] = 0;
            }
            sub_FightCount = 0;
            sub_RoundCount = 0;
            Victories = 0;
            Defeats = 0;
            DamageDone = 0;
            TotalFightTime = 0;
            TotalRoundTime = 0;
            RoundText.SetMainNumber(FightCount);
            RoundText.SetSecondaryNumber(RoundCount);
            RoundText.SetTertiaryNumber(sub_RoundCount);
        }

        //Get the actual delta time, but cap it at one-tenth of
        //a second. Except in fast mode, where we just make it
        //one-tenth of a second all the time. Note that if we make
        //this more than one-tenth of a second, we might get different
        //results in fast mode vs. normal mode by "jumping" over time
        //thresholds (cooldowns for example) that are in tenths of a second.
        if (FastMode)
            DT = 0.1f; //We could go even faster by not having visual feedback in this mode...
        else if (Time.deltaTime < 0.1f)
            DT = Time.deltaTime;
        else
            DT = 0.1f;

        //It's the start of a fight, so start a new round.
        if (RoundCount == 0)
            NewRound();

        RoundOver = IsRoundOver();
        if (RoundOver == false) //The round isn't over, so run the simulation (all the logic is in the updates of other classes).
        {
            TotalFightTime += DT; //Accumulate the SIMULATED time for telemetry data.
            TotalRoundTime += DT;
        }
        else if (RoundTimer > 0.0f) //The round is over, but this is the delay before a new round.
            RoundTimer -= DT; //Update the round delay timer.
        else //Time for a new round.
            NewRound();
    }

    //The round is over if either the player is dead or all enemies are.
    bool IsRoundOver()
    {
        if (TotalRoundTime >= MaxRoundTime)
        {
            TotalRoundTime = 0;
            return true;
        }
        //Player is dead.
        if (Player.HitPoints == 0.0f)
        {
            if (RoundOver == false) //Player just died.
            {
                SpawnInfoText("DEFEAT...");
                Defeats++;
            }
            return true;
        }
        //Enemies are dead.
        if (Player.Target == null)
        {
            if (RoundStart == true) //Make sure player has a chance to find a target at the start of a round.
                return false;
            if (RoundOver == false) //Last enemy just died.
            {
                if (MultiGroupMode)
                {
                    int res = (RoundCount % 6) - 1;
                    if (res >= 0 && res < 6)
                        EnemyDestroyed[res]++;
                    else EnemyDestroyed[5]++;
                }
                SpawnInfoText("VICTORY!!!");
                Victories++;
            }
            return true;
        }
        //Round is not over.
        RoundStart = false;
        return false;
    }

	//Reset everything for the new round.
    void NewRound()
    {
        if (FastMode && RoundCount != 0)
        {
            sub_RoundCount++;

            if (sub_RoundCount >= sub_RoundMax)
            {
                RoundCount++;
                sub_RoundCount = 0;
            }
        }
        else
        {
            RoundCount++;
        }

        RoundText.SetSecondaryNumber(RoundCount);
        RoundText.SetTertiaryNumber(sub_RoundCount);

        TotalRoundTime = 0;
        //Clear out any remaining enemies.
        ClearEnemies();
        
        //The whole fight is over, so start a new one.
        if (RoundCount > Rounds)
        {
            NewFight();
            return;
        }

        //Spawn enemies by calling the Unity engine function Instantiate().
        //Pass in the appropriate prefab, its position, its rotation (90 degrees),
        //and its parent (none).

        // CREATED PRESET GROUP ENEMY COMPOSITIONS - Jennifer Assid
        if (RoundCount % 6 == 1)
        {
            if (GroupMode)
            {
                Instantiate(EnemyType1Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType1Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType1Prefab, new Vector3(StartingX + 1.0f, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.

                EnemyDeployed[0] += 3;

            }
            else if (MultiGroupMode)
            {
                Instantiate(EnemyType1Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType5Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType1Prefab, new Vector3(StartingX, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[0]++;

            }
            else
            {
                Instantiate(EnemyType1Prefab, new Vector3(StartingX, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[0]++;

            }

        }
        else if (RoundCount % 6 == 2)
        {
            if (GroupMode)
            {
                Instantiate(EnemyType2Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Maybe adjust the starting X or Y a bit for groups.
                Instantiate(EnemyType2Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Maybe adjust the starting X or Y a bit for groups.
                Instantiate(EnemyType2Prefab, new Vector3(StartingX + 1.0f, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Maybe adjust the starting X or Y a bit for groups.

                EnemyDeployed[1] += 3;
            }
            else if (MultiGroupMode)
            {
                Instantiate(EnemyType2Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType1Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[1]++;

            }
            else
            {
                Instantiate(EnemyType2Prefab, new Vector3(StartingX, 0, 0), Quaternion.Euler(0, 0, 90), null); //Maybe adjust the starting X or Y a bit for groups.
                EnemyDeployed[1]++;
            }
        }
            
        else if (RoundCount % 6 == 3)
        {
            if (GroupMode)
            {
                Instantiate(EnemyType3Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //You'll really want these to be an array/dictionary of prefabs eventually.
                Instantiate(EnemyType3Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //You'll really want these to be an array/dictionary of prefabs eventually.
                Instantiate(EnemyType3Prefab, new Vector3(StartingX + 1.0f, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //You'll really want these to be an array/dictionary of prefabs eventually.
                EnemyDeployed[2] += 3;
            }
            else if (MultiGroupMode)
            {
                Instantiate(EnemyType5Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType3Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[2]++;

            }
            else
            {
                Instantiate(EnemyType3Prefab, new Vector3(StartingX, 0, 0), Quaternion.Euler(0, 0, 90), null); //You'll really want these to be an array/dictionary of prefabs eventually.
                EnemyDeployed[2]++;
            }
        }
        else if (RoundCount % 6 == 4)
        {
            if (GroupMode)
            {
                Instantiate(EnemyType4Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Maybe adjust the starting X or Y a bit for groups.
                Instantiate(EnemyType4Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Maybe adjust the starting X or Y a bit for groups.
                Instantiate(EnemyType4Prefab, new Vector3(StartingX + 1.0f, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Maybe adjust the starting X or Y a bit for groups.

                EnemyDeployed[3] += 3;
            }
            else if (MultiGroupMode)
            {
                Instantiate(EnemyType4Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType1Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType1Prefab, new Vector3(StartingX + 1.0f, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[3]++;

            }
            else
            {
                Instantiate(EnemyType4Prefab, new Vector3(StartingX, 0, 0), Quaternion.Euler(0, 0, 90), null); //Maybe adjust the starting X or Y a bit for groups.
                EnemyDeployed[3]++;
            }
        }
        else if (RoundCount % 6 == 5)
        {
            if (GroupMode)
            {
                Instantiate(EnemyType5Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType5Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType5Prefab, new Vector3(StartingX + 1.0f, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.

                EnemyDeployed[4] += 3;
            }
            else if (MultiGroupMode)
            {
                Instantiate(EnemyType3Prefab, new Vector3(StartingX, StartingY*0.5f, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType5Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType1Prefab, new Vector3(StartingX, -StartingY*0.5f, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[4]++;

            }
            else
            {
                Instantiate(EnemyType5Prefab, new Vector3(StartingX, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[4]++;
            }

        }
        else
        {
            if (GroupMode)
            {
                Instantiate(EnemyType6Prefab, new Vector3(StartingX, StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType6Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType6Prefab, new Vector3(StartingX + 1.0f, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.

                EnemyDeployed[5] += 3;
            }
            else if (MultiGroupMode)
            {
                Instantiate(EnemyType6Prefab, new Vector3(StartingX + 1.0f, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                Instantiate(EnemyType1Prefab, new Vector3(StartingX, -StartingY, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[5]++;

            }
            else
            {
                Instantiate(EnemyType6Prefab, new Vector3(StartingX, 0, 0), Quaternion.Euler(0, 0, 90), null); //Just make multiple calls to spawn a group of enemies.
                EnemyDeployed[5]++;
            }
        }

        /*var enemies = FindObjectsOfType<Enemy>();
        if (enemies.Length != 0) //No enemies means no target.
        {
            foreach (Enemy enemy in enemies)
            {
                EnemyDeployed[enemy.Type]++;
            }
        }*/
        //Note that this just cycles through enemy types, but you'll need more structure than this.
        //Each fight should be one AI type against one enemy type multiple times. And then each AI type
        //against a group of the same type multiple times. And then each AI type against a mixed group
        //multiple times. And possibly more.

        //Call the Initialize() functions for the player.
        Player.Initialize();

        //Feedback is good...
        SpawnInfoText("ROUND " + RoundCount); //Look! A string concatenation operator!

        //Reset the round delay timer (and round start flag) for after this new round ends.
        RoundTimer = RoundDelay;
        RoundStart = true;
    }

    //Reset everything for the new fight.
    void NewFight()
    {
        bool old_MultiGroup = MultiGroupMode;
        //sub_FightCount = 0;

        if (FastMode)
        {
            if (GroupMode == false && MultiGroupMode == false)
            {
                FightCount--;
                GroupMode = true;
                sub_FightCount = 1;
            }
            else if (GroupMode == true && MultiGroupMode == false)
            {
                FightCount--;
                GroupMode = false;
                MultiGroupMode = true;
                sub_FightCount = 2;
            }
            else
            {
                MultiGroupMode = false;
                GroupMode = false;
                sub_FightCount = 0;
            }
        }

        
        //foreach (int i in EnemyDestroyed) EnemyDeployed[i] = 0;

        FightCount++;
        RoundCount = 0;

        RoundText.SetMainNumber(FightCount);
        RoundText.SetSecondaryNumber(RoundCount);
        RoundText.SetTertiaryNumber(sub_RoundCount);


        //Show a bit of telemetry data on screen.
        SpawnInfoText(Victories + "-" + Defeats + "\n" + DamageDone / TotalFightTime + " DPS");
        //Write all the telemetry data to the file.
        DataStream.WriteLine(CurrentAI + "," + Victories + "," + Defeats + "," + DamageDone / TotalFightTime + "," + TotalFightTime / Rounds + "," + AbilityOneUseCount + "," + AbilityTwoUseCount + "," + AbilityThreeUseCount + "," + AbilityFourUseCount + "," + AbilityFiveUseCount 
             + "," + EnemyDestroyed[0] + "," + EnemyDeployed[0] + "," + EnemyDestroyed[1] + "," + EnemyDeployed[1] + "," + EnemyDestroyed[2] + "," + EnemyDeployed[2] + "," + EnemyDestroyed[3] + "," + EnemyDeployed[3] + "," + EnemyDestroyed[4] + "," + EnemyDeployed[4] + "," + EnemyDestroyed[5] + "," + EnemyDeployed[5] + "," + old_MultiGroup);
        //Reset the telemetry counters
        Victories = 0;
        Defeats = 0;
        DamageDone = 0;
        TotalFightTime = 0;
        AbilityOneUseCount = AbilityTwoUseCount = AbilityThreeUseCount = AbilityFourUseCount = AbilityFiveUseCount = 0;
        for (int i = 0; i < 6; i++)
        {
            EnemyDeployed[i] = 0;
            EnemyDestroyed[i] = 0;
        }
        //After the first fight (which is random), just spam a single key for each fight.
        if (FightCount % 9 == 0 || FightCount == 0)
        {
            CurrentAI = "Random";
        }
        else if (FightCount % 9 == 1) // Player does nothing
        {
            CurrentAI = "Intelligent";
        }
        else if (FightCount % 9 == 2) // Player spams one ability
        {
            CurrentAI = "Spam 1";
        }
        else if (FightCount % 9 == 3) 
        {
            CurrentAI = "Spam 2";
        }
        else if (FightCount % 9 == 4) 
        {
            CurrentAI = "Spam 3";
        }
        else if (FightCount % 9 == 5)
        {
            CurrentAI = "Spam 4";
        }
        else if (FightCount % 9 == 6)
        {
            CurrentAI = "Spam 5";
        }
        else if (FightCount % 9 == 7) // Whichever becomes available first
        {
            CurrentAI = "First available";
        }
        else // Intelligent
        {
            CurrentAI = "Nothing";
        }
    }

    //Destroy all the enemy game objects.
    void ClearEnemies()
    {
        //Find all the game objects that have an Enemy component.
        var enemies = FindObjectsOfType<Enemy>();
        if (enemies.Length == 0) //Didn't find any.
            return;
        foreach (Enemy enemy in enemies) //A foreach loop! Fancy...
            Destroy(enemy.gameObject);
    }

    //Spawn text at the center of the screen.
    //If set to static, that just means it doesn't move.
    void SpawnInfoText(string text, bool isStatic = false)
    {
        if (FastMode && isStatic == false) return;
        SpawnInfoText(new Vector3(0, 0, 0), text, isStatic);
    }

    //Spawn text wherever you want.
    //If set to static, that just means it doesn't move.
    void SpawnInfoText(Vector3 location, string text, bool isStatic = false)
    {
        if (FastMode && isStatic == false) return;
        //Throw up some text by calling the Unity engine function Instantiate().
        //Pass in the appropriate InfoText prefab, its position, its rotation (none in this case),
        //and its parent (the canvas because this is text). Then we get the
        //Text component from the new game object in order to set the text itself.
        Text infotext;
        if (isStatic)
            infotext = Instantiate(StaticInfoTextPrefab, location, Quaternion.identity, Canvas.transform).GetComponent<Text>();
        else
            infotext = Instantiate(InfoTextPrefab, location, Quaternion.identity, Canvas.transform).GetComponent<Text>();
        //Set the text.
        infotext.text = text;
    }
}