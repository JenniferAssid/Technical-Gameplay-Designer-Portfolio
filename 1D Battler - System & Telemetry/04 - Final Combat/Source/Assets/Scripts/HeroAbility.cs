/*******************************************************************************
Author:    Benjamin Ellinger
DP Email:  bellinge@digipen.edu
Date:      2/1/2022
Course:    DES 212

Description:
	This file is part of the framework for the 1D Combat Simulator assignment in
	DES 212 (System Design Methods). It can be freely used and modified by students
	for that assignment.
	
    This component makes a game object a hero ability. The game object then must
    be parented to the actual hero game object in order to work. Should this really
    be a different class than the enemy ability? It doesn duplicate some functionality,
    but often hero and enemy abilities end up subtly different, so this can be okay to do.
	
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
public class HeroAbility : MonoBehaviour
{
    //Properties that define the ability's cooldown time, damage done, power used, range, etc.
    public float CooldownTime = 1.0f;
    public float PowerRestored = 1.0f;
    public float DamageDone = 1.0f;
    public float DamageReduction = 0.0f;
    public float PowerUsed = 1.0f;
    public float HealthUsed = 1.0f;
    public float MaximumRange = 10.0f;
    public float MaximumRadius = 0.0f;
    public bool Inactive = false; //Make an ability inactive to temporarily or permanently not have it used.

    [HideInInspector]
    public float CooldownLeft = 0.0f; //How much of the cooldown time is actually left.

    [HideInInspector]
    public BarScaler CooldownBar; //Reference to the cooldown timer bar, so we don't have to look it up all the time.

    [HideInInspector]
    public Hero ParentHero; //Reference to the parent hero, so we don't have to look it up all the time.

    //Start is called before the first frame update
    void Start()
    {
        //Get the parent.
        ParentHero = GameObject.Find("Hero").GetComponent<Hero>();
        //Find the cooldown timer gameobject, which must be a child of this object.
        CooldownBar = transform.Find("Cooldown").GetComponent<BarScaler>();

        // ADDED STATISTICS THAT THE PLAYER CAN READ IN THE UI - Jennifer Assid
        Text abilityText = gameObject.transform.Find("Description").GetComponent<Text>();
        abilityText.text = "Cooldown: " + CooldownTime + "s\n";

        if (PowerRestored != 0.0f)
        {
            abilityText.text += "Patience Restored: " + PowerRestored + "\n";
        }
        if (DamageDone != 0.0f)
        {
            abilityText.text += "Damage Dealt: " + DamageDone + "\n";
        }
        if (DamageReduction != 0.0f)
        {
            abilityText.text += "Damage Reduction: +" + (DamageReduction * 100.0f) + "%\n";
        }
        if (PowerUsed != 0.0f)
        {
            abilityText.text += "Power Used: " + PowerUsed + "\n";
        }
        if (HealthUsed != 0.0f)
        {
            if (HealthUsed < 0.0f)
            {
                abilityText.text += "Health Restored: " + -HealthUsed + "\n";
            }
            else
            {
                abilityText.text += "Health Used: " + HealthUsed + "\n";
            }
        }
        if (MaximumRange != 0.0f)
        {
            abilityText.text += "Maximum Range: " + MaximumRange + "\n";
        }
        if (MaximumRadius != 0.0f)
        {
            abilityText.text += "Maximum Radius: " + MaximumRadius + "\n";
        }
    }

    //Update is called once per frame
    void Update()
    {
        CooldownLeft = Mathf.Clamp(CooldownLeft - SimControl.DT, 0.0f, CooldownTime);

        if (SimControl.FastMode) return;

        Text descriptionText = gameObject.transform.Find("Description").GetComponent<Text>();
        Text abilityText = gameObject.GetComponent<Text>();
        Text abilityNumberText = gameObject.transform.Find("AbilityNumber").GetComponent<Text>();
        //Don't let the cooldown amount left go below zero.
        
        //Since cooldowns update every frame, no need to worry about interpolating over time.
        if (Inactive || CooldownTime == 0.0f) //Either doesn't have a cooldown or is inactive, so scale it to nothing.
        {
            CooldownBar.InterpolateToScale(0.0f, 0.0f);

            // ADDED ABILITY CIRCLES FOR PLAYER TO UNDERSTAND DISTANCES - Jennifer Assid
            descriptionText.color = abilityText.color = abilityNumberText.color = Color.gray;
            if (MaximumRadius != 0.0f)
            {
                Color tmp = ParentHero.Target.transform.Find("Ring").GetComponent<SpriteRenderer>().color;
                tmp = Color.gray;
                ParentHero.Target.transform.Find("Ring").GetComponent<SpriteRenderer>().color = tmp;
            }
        }
        else
        {
            CooldownBar.InterpolateToScale(CooldownLeft / CooldownTime, 0.0f);
            if (ParentHero.Target == null) return;

            // ADDED ABILITY CIRCLES FOR PLAYER TO UNDERSTAND DISTANCES - Jennifer Assid
            if (CooldownLeft == 0.0f && DistanceToTarget() <= MaximumRange && ParentHero.GetHitPoints() > HealthUsed && ParentHero.GetPower() >= PowerUsed)
            {
                descriptionText.color = abilityText.color = abilityNumberText.color = Color.green;
                if (MaximumRadius != 0.0f)
                {
                    Color tmp = ParentHero.Target.transform.Find("Ring").GetComponent<SpriteRenderer>().color;
                    tmp = Color.green;
                    ParentHero.Target.transform.Find("Ring").GetComponent<SpriteRenderer>().color = tmp;
                }
            }
            else
            {
                descriptionText.color = abilityText.color = abilityNumberText.color = Color.gray;
                if (MaximumRadius != 0.0f)
                {
                    Color tmp = ParentHero.Target.transform.Find("Ring").GetComponent<SpriteRenderer>().color;
                    tmp = Color.gray;
                    ParentHero.Target.transform.Find("Ring").GetComponent<SpriteRenderer>().color = tmp;
                }
            }
        }
    }

    //Don't let a cooldown affect the next fight
    public void ResetCooldown()
    {
        CooldownLeft = 0.0f;
    }

    public float GetCooldownLeft()
    {
        return CooldownLeft;
    }

    public float GetCooldownMax()
    {
        return CooldownTime;
    }

    //Get the distance to the target along the X axis (1D not 2D).
    public float DistanceToTarget()
    {
        return Mathf.Abs(ParentHero.transform.position.x - ParentHero.Target.transform.position.x);
    }

    // Is an ability ready for use?
    public bool IsReady()
    {
        //It's inactive.
        if (Inactive)
            return false;
        //I'm dead.
        if (ParentHero.HitPoints == 0.0f)
            return false;
        //No target.
        if (ParentHero.Target == null)
            return false;
        //Dead target.
        if (ParentHero.Target.HitPoints == 0.0f)
            return false;
        //Target too far away.
        if (DistanceToTarget() > MaximumRange)
            return false;
        //Still on cooldown.
        if (CooldownLeft > 0.0f)
            return false;
        //Not enough power.
        if (PowerUsed > ParentHero.Power)
            return false;
        //Ready to go.
        return true;
    }

    //Use the ability if it is ready.
    public bool Use()
    {
        //Is it ready?
        if (IsReady() == false)
            return false;
        //Use the power.
        ParentHero.UsePower(PowerUsed);
        ParentHero.UseHealth(HealthUsed);
        //Apply the damage (or healing is the damage is negative).
        if (ParentHero.Target.TakeDamage(DamageDone) == true)
            ParentHero.Target = ParentHero.FindTarget(); //If the target is dead, find a new one.

        ParentHero.RestorePower(PowerRestored);

        if (ParentHero.GetApplyDeducation() == true && ParentHero.Target != null)
        {
            ParentHero.Target.GetAbilityOne().SetDamageDeduction(DamageReduction);
        }

        if (ParentHero.applyDistraction == true && ParentHero.Target != null)
        {
            //Find all the enemies in the scene.
            var enemies = FindObjectsOfType<Enemy>();
            if (enemies.Length == 0) //No enemies means no target.
                return false;

            for (int i = 0; i < enemies.Length; i++)
            {
                float areaOfEffect = ParentHero.Target.transform.position.x - enemies[i].transform.position.x;
                if (Mathf.Abs(areaOfEffect) <= MaximumRadius)
                    enemies[i].isDistracted = true;
            }

            ParentHero.Target.isDistracted = true;
        }

        //TODO: Add needed flags or other functionality for abilities that don't just do
        //damage or affect more than one target (AoE, heals, dodges, blocks, stuns, etc.)

        //Put the ability on cooldown.
        CooldownLeft = CooldownTime;
        return true;
    }
}
