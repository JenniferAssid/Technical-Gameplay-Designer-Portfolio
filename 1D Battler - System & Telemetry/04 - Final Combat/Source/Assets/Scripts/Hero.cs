/*******************************************************************************
Author:    Benjamin Ellinger
DP Email:  bellinge@digipen.edu
Date:      2/1/2022
Course:    DES 212

Description:
	This file is part of the framework for the 1D Combat Simulator assignment in
	DES 212 (System Design Methods). It can be freely used and modified by students
	for that assignment.
	
    This component makes a game object a hero that can be controlled by the player.
    There is only a single hero that is already placed in the scene.
	
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
using UnityEngine; //The library that lets your access all of the Unity functionality.
using UnityEngine.UI; //This is here so we don't have to type out longer names for UI components.

//Inherits from MonoBehavior like all normal Unity components do...
//Remember that the class name MUST be identical to the file name!
public class Hero : MonoBehaviour
{
    //Properties for maximum hit points, movement speed, maximum power, and optimal range.
    public float MaxHitPoints = 200;
    public float MoveSpeed = 0.1f;
    public float MaxPower = 100;
    public float OptimalRange = 5.0f;

    [HideInInspector]
    public float HitPoints = 200; //Current hit points
    [HideInInspector]
    public float Power = 10; //Current power

    [HideInInspector]
    private int enemyIndex = 0;

    [HideInInspector]
    public Enemy Target; //Current target enemy.

    [HideInInspector]
    public bool applyDeducation = false;
    [HideInInspector]
    public bool applyDistraction = false;

    //References to the health and power UI bars, so we don't have to look them up all the time.
    [HideInInspector]
    public BarScaler HealthBar;
    [HideInInspector]
    public BarScaler PowerBar;
    [HideInInspector]
    public FractionUpdate HealthFraction;
    [HideInInspector]
    public FractionUpdate PowerFraction;

    //References to the abilities, so we don't have to look them up all the time.
    //These are set by hand in the inspector for the hero game object.
    public HeroAbility AbilityOne;
    public HeroAbility AbilityTwo;
    public HeroAbility AbilityThree;
    public HeroAbility AbilityFour;
    public HeroAbility AbilityFive;

    //Start is called before the first frame update
    void Start()
    {
        //The static version of Find() on the GameObject class will just find the named object anywhere.
        //Use GetComponent so we don't have to use it later to access the functionality we want.
        HealthBar = GameObject.Find("HeroResources/HealthBar").GetComponent<BarScaler>(); 
        PowerBar = GameObject.Find("HeroResources/PowerBar").GetComponent<BarScaler>();
        HealthFraction = GameObject.Find("WillpowerFraction").GetComponent<FractionUpdate>();
        PowerFraction = GameObject.Find("PatienceFraction").GetComponent<FractionUpdate>();
        HealthFraction.denominator = MaxHitPoints;
        PowerFraction.denominator = MaxPower;
    }

    //Update is called once per frame
    void Update()
    {
        if (SimControl.RoundOver) //Don't update between rounds (or when the sim is over).
            return;
        if (Target == null) //If we don't have a target, the round must have just started.
            Initialize();
        //The fight is on, so move and use abilities.
        DoMovement();

        // ADDED CUSTOM AI FOR TELEMTRY TESTING - Jennifer Assid
        if (SimControl.AutoMode == true) //Let an "AI" determine which abilities to use.
        {
            if (SimControl.CurrentAI == "Random")
                UseRandomAbility();
            else if (SimControl.CurrentAI == "Nothing")
            {
                // Do nothing
            }
            else if (SimControl.CurrentAI == "Spam 1")
                UseAbility(1);
            else if (SimControl.CurrentAI == "Spam 2")
                UseAbility(2);
            else if (SimControl.CurrentAI == "Spam 3")
                UseAbility(3);
            else if (SimControl.CurrentAI == "Spam 4")
                UseAbility(4);
            else if (SimControl.CurrentAI == "Spam 5")
                UseAbility(5);
            else if (SimControl.CurrentAI == "First available")
            {
                if (AbilityOne.CooldownLeft == 0.0f) UseAbility(1);
                else if (AbilityTwo.CooldownLeft == 0.0f) UseAbility(2);
                else if (AbilityThree.CooldownLeft == 0.0f) UseAbility(3);
                else if (AbilityFour.CooldownLeft == 0.0f) UseAbility(4);
                else if (AbilityFive.CooldownLeft == 0.0f) UseAbility(5);
            }
            else if (SimControl.CurrentAI == "Intelligent")
            {
                if (SimControl.GroupMode || SimControl.MultiGroupMode)
                    UseAbility(5);

                UseAbility(2);
                UseAbility(3);
                UseAbility(4);
                UseAbility(1);
            }
        }
        else //Let the player select which abilities to use.
        {
            if (Input.GetKeyDown(KeyCode.Alpha1) == true)
                UseAbility(1);
            if (Input.GetKeyDown(KeyCode.Alpha2) == true)
                UseAbility(2);
            if (Input.GetKeyDown(KeyCode.Alpha3) == true)
                UseAbility(3);
            if (Input.GetKeyDown(KeyCode.Alpha4) == true)
                UseAbility(4);
            if (Input.GetKeyDown(KeyCode.Alpha5) == true)
                UseAbility(5);

            // GAVE PLAYER ABILITY TO TARGET DIFFERENT ENEMIES - Jennifer Assid
            // Change enemy (go up the list)
            if (Input.GetKeyDown(KeyCode.DownArrow) == true)
            {
                //Find all the enemies in the scene.
                var enemies = FindObjectsOfType<Enemy>();
                if (enemies.Length == 0) //No enemies means no target.
                    return;

                if (enemies.Length == 1)
                {
                    Target = enemies[0];
                    return;
                }

                Target.isTargeted = false;

                enemyIndex++;
                if (enemyIndex >= enemies.Length) enemyIndex = 0;

                if (enemies[enemyIndex].HitPoints > 0) Target = enemies[enemyIndex];
                else Target = null;

                if (Target) Target.isTargeted = true;
            }
            // Change enemy (go up the list)
            if (Input.GetKeyDown(KeyCode.UpArrow) == true)
            {
                //Find all the enemies in the scene.
                var enemies = FindObjectsOfType<Enemy>();
                if (enemies.Length == 0) //No enemies means no target.
                    return;

                if (enemies.Length == 1)
                {
                    Target = enemies[0];
                    return;
                }

                Target.isTargeted = false;

                enemyIndex--;
                if (enemyIndex < 0) enemyIndex = enemies.Length - 1;

                if (enemies[enemyIndex].HitPoints > 0) Target = enemies[enemyIndex];
                else Target = null;

                if (Target) Target.isTargeted = true;
            }
        }

        HealthFraction.numerator = HitPoints;
        PowerFraction.numerator = Power;
    }

    //Try to stay close to optimal range. Note this is done even in Auto mode.
    public void DoMovement()
    {
        if (HitPoints <= 0.0f || Target == null) //If all enemies or the player is dead, no need to move.
            return;
        
        if (SimControl.AutoMode == true && SimControl.CurrentAI != "Nothing")
        {
            //Calculate distance to target along the X axis (1D not 2D).
            float distanceToTarget = transform.position.x - Target.transform.position.x;
            //If we are between 80% and 100% of optimal range, that's good enough.
            /*if (Mathf.Abs(distanceToTarget) >= 1.0f)
                return;*/
            //If we are too close, flip the "distance" so we will move away instead of towards.
            if (Mathf.Abs(distanceToTarget) < 1.0f)
                distanceToTarget = -distanceToTarget;
            //We need to move, so get our current X position.
            float newX = transform.position.x;
            if (distanceToTarget > 0) //Move to the left.
                newX -= MoveSpeed * SimControl.DT; //Make sure to use the simulated DT.
            else //Move to the right.
                newX += MoveSpeed * SimControl.DT; //Make sure to use the simulated DT.
                                                   //Don't go past the edge of the arena.
            newX = Mathf.Clamp(newX, -SimControl.EdgeDistance, SimControl.EdgeDistance);
            //Update the transform.
            transform.position = new Vector3(newX, transform.position.y, transform.position.z);
        }
        else
        {
            if (Input.GetKey(KeyCode.RightArrow))
            {
                float distanceToTarget = transform.position.x - Target.transform.position.x;
                //If we are too close, flip the "distance" so we will move away instead of towards.
                float newX = transform.position.x;
                if (Mathf.Abs(distanceToTarget) >= 1.0f)
                    newX = transform.position.x + MoveSpeed * SimControl.DT;
                transform.position = new Vector3(Mathf.Clamp(newX, -SimControl.EdgeDistance, SimControl.EdgeDistance), transform.position.y, transform.position.z);
            }
            if (Input.GetKey(KeyCode.LeftArrow))
            {
                float distanceToTarget = transform.position.x - Target.transform.position.x;
                //If we are too close, flip the "distance" so we will move away instead of towards.
                float newX = transform.position.x - MoveSpeed * SimControl.DT;
                transform.position = new Vector3(Mathf.Clamp(newX, -SimControl.EdgeDistance, SimControl.EdgeDistance), transform.position.y, transform.position.z);
            }
        }
    }

    // HELPER FUNCTION - Jennifer Assid
    //Find the best target for the hero.
    public Enemy FindTarget()
    {
        //Find all the enemies in the scene.
        var enemies = FindObjectsOfType<Enemy>();
        if (enemies.Length == 0) //No enemies means no target.
            return null;
        //There are enemies, now find the best one.
        Enemy target = null;
        if (Target != null && Target.HitPoints > 0.0f) //Start with our current target if it is still alive.
            target = Target;

        //Find the enemy with the lowest HP.
        float lowestHP = float.MaxValue;
        if (target) //Start with the current target so any ties don't cause target switching.
            lowestHP = target.HitPoints;
        //Loop through all the enemies to find the weakest enemy.
        enemyIndex = 0;
        foreach (Enemy enemy in enemies)
        {
            enemyIndex++;
            if (enemy.HitPoints > 0 && enemy.MaxHitPoints < lowestHP)
            {
                target = enemy;
                lowestHP = enemy.MaxHitPoints;
            }
        }

        if (target) target.isTargeted = true;
        return target;
    }

    //This is NOT a Start() function because we need to be able to call Initialize() whenever a new
    //round starts, not just when the object is created.
    public void Initialize()
    {
        //Set our X position to the correct starting position on the left side of the arena, while keeping the Y and Z the same.
        transform.position = new Vector3(-SimControl.StartingX, transform.position.y, transform.position.z);
        //Reset hit points.
        HitPoints = MaxHitPoints;
        //Reset power, but to 10% of MaxPower, not the full amount.
        Power = MaxPower * 0.1f;
        //Reset all the cooldowns.
        if (AbilityOne != null) AbilityOne.ResetCooldown();
        if (AbilityTwo != null) AbilityTwo.ResetCooldown();
        if (AbilityThree != null) AbilityThree.ResetCooldown();
        if (AbilityFour != null) AbilityFour.ResetCooldown();
        if (AbilityFive != null) AbilityFive.ResetCooldown();
        //Find a target.
        Target = FindTarget();
        //Make sure the health and power bars get reset.
        HealthBar.InterpolateImmediate(HitPoints / MaxHitPoints);
        PowerBar.InterpolateImmediate(Power / MaxPower);
    }
    
    //Try to use a random ability.
    public bool UseRandomAbility()
    {
        //Get a random number between 1 and 4. Yes, the integer version of this function is not
        //inclusive. This is wrong and Unity should feel bad for doing this.
        return UseAbility(Random.Range(1, 6));
    }

    //Try to use a specific ability.
    public bool UseAbility(int abilityNumber)
    {
        // ADDED CUSTOM FUNCTIONALITY FOR EACH ABILITY - Jennifer Assid
        if (abilityNumber == 1 && AbilityOne != null) // Standard damage
        {
            applyDeducation = false;
            applyDistraction = false;
            bool res = AbilityOne.Use();
            if (res) SimControl.AbilityOneUseCount++;
            return res;
        }
        if (abilityNumber == 2 && AbilityTwo != null) // Restore power (in exchange for health)
        {
            applyDeducation = false;
            applyDistraction = false;
            bool res = AbilityTwo.Use();
            if (res) SimControl.AbilityTwoUseCount++;
            return res;
        }
        if (abilityNumber == 3 && AbilityThree != null) // Reduce the enemy's main attack
        {
            applyDeducation = true;
            applyDistraction = false;
            bool res = AbilityThree.Use();
            if (res) SimControl.AbilityThreeUseCount++;
            return res;
        }
        if (abilityNumber == 4 && AbilityFour != null) // Finisher - larger damage - longer cooldown
        {
            applyDeducation = false;
            applyDistraction = false;
            bool res = AbilityFour.Use();
            if (res) SimControl.AbilityFourUseCount++;
            return res;
        }
        if (abilityNumber == 5 && AbilityFive != null) // Finisher - larger damage - longer cooldown
        {
            applyDeducation = false;
            applyDistraction = true;
            bool res = AbilityFive.Use();
            if (res) SimControl.AbilityFiveUseCount++;
            return res;
        }
        return false;
    }

    public bool GetApplyDeducation()
    {
        return applyDeducation;
    }

    //Use a given amount of power.
    public void UsePower(float power)
    {
        //Make sure power does not go negative (or above max, becaust the "power" could be negative).
        Power = Mathf.Clamp(Power - power, 0.0f, MaxPower);
        //Interpolate the power UI bar over half a second.
        PowerBar.InterpolateToScale(Power / MaxPower, 0.5f);
    }

    // Use a given amonut of health (in exchange for a power - maybe some different text?
    public void UseHealth(float health)
    {
        // Make sure that the health does not go into the negative (or above the max)
        HitPoints = Mathf.Clamp(HitPoints - health, 0.0f, MaxHitPoints);
        // Interpolate the health UI bare over half a second
        HealthBar.InterpolateToScale(HitPoints / MaxHitPoints, 0.5f);

        if (!SimControl.FastMode && health != 0.0f)
        {
            Text healthText = Object.Instantiate(SimControl.InfoTextPrefab, transform.position, Quaternion.identity, SimControl.Canvas.transform).GetComponent<Text>();
            
            if (health < 0.0f)
            {
                healthText.text = "+" + Mathf.Floor(health * -1.0f);
                healthText.color = Color.green;
            }
            else
            {
                healthText.text = "-" + Mathf.Floor(health);
            }
        }
    }

    public float GetHitPoints()
    {
        return HitPoints;
    }

    public float GetPower()
    {
        return Power;
    }


    //Take damage from any source.
    public bool TakeDamage(float damage)
    {
        if (damage != 0.0f) //Don't bother if the damage is 0
        {
            //Make sure hit points do not go negative (or above max, because the "damage" could be negative, i.e., healing).
            HitPoints = Mathf.Clamp(HitPoints - damage, 0.0f, MaxHitPoints);
            //Interpolate the hit point UI bar over half a second.
            HealthBar.InterpolateToScale(HitPoints / MaxHitPoints, 0.5f);
            //Create a temporary InfoText object to show the damage using the static Instantiate() function.
            
            if (!SimControl.FastMode)
            {
                Text damageText = Object.Instantiate(SimControl.InfoTextPrefab, transform.position, Quaternion.identity, SimControl.Canvas.transform).GetComponent<Text>();
                //Set the damage text to just the integer amount of the damage done.
                //Uses the "empty string plus number" trick to make it a string.
                damageText.text = "-" + Mathf.Floor(damage);
            }
        }
        //Return true if dead.
        return (HitPoints <= 0.0f);
    }

    // Restore health from any source
    public bool RestorePower(float power)
    {
        if (power != 0.0f)
        {
            Power = Mathf.Clamp(Power + power, 0.0f, MaxPower);
            PowerBar.InterpolateToScale(Power / MaxPower, 0.5f);
            
            if (!SimControl.FastMode)
            {
                Text powerText = Object.Instantiate(SimControl.InfoTextPrefab, transform.position, Quaternion.identity, SimControl.Canvas.transform).GetComponent<Text>();
                powerText.color = Color.blue;
                powerText.text = "+" + Mathf.Floor(power);
            }
        }

        return (Power >= 0.0f);
    }

}

