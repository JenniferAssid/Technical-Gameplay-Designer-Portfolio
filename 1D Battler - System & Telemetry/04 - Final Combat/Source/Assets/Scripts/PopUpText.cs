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
using UnityEngine.EventSystems;

public class PopUpText : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    private bool mouse_over = false;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Text details = gameObject.transform.Find("Details").GetComponent<Text>();

        if (mouse_over)
        {
            details.color = new Color(0.0f, 0.0f, 0.0f, 0.0f);
        }
        else
        {
            details.color = Color.gray;
        }
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        mouse_over = true;
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        mouse_over = false;
    }
}
