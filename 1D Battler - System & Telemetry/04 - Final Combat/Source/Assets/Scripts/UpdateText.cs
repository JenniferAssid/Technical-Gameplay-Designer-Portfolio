/*******************************************************************************
Author:    Jennifer Assid
Date:      Spring 2022
Course:    DES 212

Description:
	A helper file for fraction updates in the UI
	
*******************************************************************************/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI; //This is here so we don't have to type out longer names for UI components.

public class UpdateText : MonoBehaviour
{
    public string DefaultText = "Default: ";
    public static int MainNumber = 0;
    public static int SecondaryNumber = 0;
    public static int TertiaryNumber = 0;
    static public Text TextComponent;

    [HideInInspector]
    public bool changed = false;

    // Start is called before the first frame update
    void Start()
    {
        TextComponent = gameObject.GetComponent<Text>();
        if (TextComponent == null) return;
        TextComponent.text = DefaultText + MainNumber + ":" + SecondaryNumber + "." + TertiaryNumber;
    }

    public void SetMainNumber(int mainNumber)
    {
        changed = true;
        MainNumber = mainNumber;
    }

    public void SetSecondaryNumber(int secondaryNumber)
    {
        changed = true;
        SecondaryNumber = secondaryNumber;
    }

    public void SetTertiaryNumber(int tertiaryNumber)
    {
        changed = true;
        TertiaryNumber = tertiaryNumber;
    }

    // Update is called once per frame
    void Update()
    {
        if (changed)
        {
            if (TextComponent == null) return;
            TextComponent.text = DefaultText + MainNumber + ":" + SecondaryNumber + "." + TertiaryNumber;
            changed = false;
        }
    }
}
