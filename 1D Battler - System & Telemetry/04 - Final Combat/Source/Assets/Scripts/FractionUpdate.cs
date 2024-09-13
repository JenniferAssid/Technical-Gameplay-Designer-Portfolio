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
using UnityEngine.UI;

public class FractionUpdate : MonoBehaviour
{
    [HideInInspector]
    public float numerator = 0.0f;
    public float denominator = 0.0f;

    // Start is called before the first frame update
    void Start()
    {
        Text text_ = gameObject.GetComponent<Text>();
        if (!text_) return;

        text_.text = numerator + " / " + denominator;
    }

    // Update is called once per frame
    void Update()
    {
        if (SimControl.FastMode) return;
        Text text_ = gameObject.GetComponent<Text>();
        if (!text_) return;

        text_.text = numerator + " / " + denominator;
    }
}
