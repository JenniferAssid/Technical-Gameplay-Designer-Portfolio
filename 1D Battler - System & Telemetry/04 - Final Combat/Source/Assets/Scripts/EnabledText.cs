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

public class EnabledText : MonoBehaviour
{
    [HideInInspector]
    public bool isEnabled = false;

    // Start is called before the first frame update
    void Start()
    {
        Text text = gameObject.GetComponent<Text>();
        if (text == null) return;
        text.color = Color.gray;
    }

    // Update is called once per frame
    void Update()
    {
        if (SimControl.FastMode) return;
        Text text = gameObject.GetComponent<Text>();
        if (text == null) return;
        if (isEnabled)
        {
            text.color = Color.green;
        }
        else
        {
            text.color = Color.gray;
        }
    }
}
