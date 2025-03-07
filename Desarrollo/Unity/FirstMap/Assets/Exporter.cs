﻿using UnityEngine;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using System;
using System.Linq;
using System.Collections;


public class Exporter : MonoBehaviour
{


    public string fileName = "";

    public Objeto createObject(GameObject g)
    {
        Objeto objeto = new Objeto();
        objeto.nombre = g.name;
        objeto.posX = g.transform.position.x;
        objeto.posY = g.transform.position.y;
        objeto.posZ = g.transform.position.z;

        objeto.sizeX = g.transform.lossyScale.x;
        objeto.sizeY = g.transform.lossyScale.y;
        objeto.sizeZ = g.transform.lossyScale.z;

        objeto.rotX = g.transform.rotation.eulerAngles.x;
        objeto.rotY = g.transform.rotation.eulerAngles.y;
        objeto.rotZ = g.transform.rotation.eulerAngles.z;

        objeto.forwardX = g.transform.forward.x;
        objeto.forwardY = g.transform.forward.y;
        objeto.forwardZ = g.transform.forward.z;


        objeto.tag = g.tag;

        if (g.transform.childCount == 1)
        {
            GameObject child = g.transform.GetChild(0).gameObject;
            if (child.GetComponent<Renderer>().material.mainTexture != null)
            {
                //Debug.Log(child.GetComponent<Renderer>().material.mainTexture.name);
                objeto.textura = child.GetComponent<Renderer>().material.mainTexture.name;
                //Debug.Log(child.GetComponent<MeshFilter>().name);
                objeto.mesh = child.GetComponent<MeshFilter>().name;
            }

        }


        multipleTags mt = g.GetComponent<multipleTags>();
        if (mt != null)
        {
            objeto.extraTags = mt.extratags.ToString();
        }
        BoxCollider col = g.GetComponentInChildren<BoxCollider>();
        if (col != null)
        {
            objeto.colliderX = col.center.x;
            objeto.colliderY = col.center.y;
            objeto.colliderZ = col.center.z;

            objeto.colliderSizeX = col.size.x;
            objeto.colliderSizeY = col.size.y;
            objeto.colliderSizeZ = col.size.z;
        }
        else
        {
            objeto.masa = 0;
            objeto.colliderX = 0;
            objeto.colliderY = 0;
            objeto.colliderZ = 0;

            objeto.colliderSizeX = objeto.sizeX;
            objeto.colliderSizeY = objeto.sizeY;
            objeto.colliderSizeZ = objeto.sizeZ;
        }

        return objeto;
    }


    // Use this for initialization
    void Start()
    {
        object[] obj = FindObjectsOfType(typeof(GameObject));

        Raiz raiz = new Raiz();

        foreach (object o in obj)
        {
            GameObject g = (GameObject)o;
            if (!g.name.Equals("Script") && !g.name.Equals("Main Camera")  && !g.name.Equals("Waypoint Editor"))
            {
                GameObject parent = g.transform.parent.gameObject;
                if (parent != null)
                {
                    if (parent.name.Equals("Script"))
                    {
                        Objeto objeto = createObject(g);


                        if (g.tag == "Zona")
                        {
                            int total= g.transform.childCount;
                            for(int i=0;i<total;i++)
                            {
                                GameObject child = g.transform.GetChild(i).gameObject;
                                objeto.o_children.Add(createObject(child));
                            }
                        }

                        if (g.tag == "Grafo")
                        {
                            WayPoint[] w = g.GetComponentsInChildren<WayPoint>();
                           // WayPoint w = g.GetComponent<WayPoint>();
                           foreach(WayPoint waypoint in w) {
                                Nodo nodo = new Nodo(waypoint.name,waypoint.tag);
                                nodo.posX = waypoint.transform.position.x;
                                nodo.posY = waypoint.transform.position.y;
                                nodo.posZ = waypoint.transform.position.z;
                                Debug.Log(waypoint.name);
                                foreach (WayPointPercent p in waypoint.outs) {
                                    objeto.hasChild = true;

                                    nodo.conexiones.Add(Int32.Parse(p.waypoint.name));

                                }
                                //if(nodo.conexiones.Count>0)
                                    objeto.n_children.Add(nodo);

                            }
                        }
                        if (g.tag == "PointLight" || g.tag == "DirectionalLight")
                        {
                            Light l = (Light)g.GetComponent(typeof(Light));
                            objeto.color_r = l.color.r;
                            objeto.color_g = l.color.g;
                            objeto.color_b = l.color.b;
                        }
                     
                        raiz.addChild(objeto);
                    }
                }
            }
        }

        if (fileName == "")
        {
            fileName = "C:\\defaultMapName.lbmap";
        }
        var sr = File.CreateText(fileName);
        sr.WriteLine(raiz.toJson());
        sr.Close();

    }

    // Update is called once per frame
    void Update()
    {

    }
}

[System.Serializable]
public class Zona
{
    public string nombre;
    public float posX, posY, posZ;
    public float sizeX, sizeY, sizeZ;

    public List<Nodo> children;
    public Zona()
    {
        children = new List<Nodo>();
    }

}



public class Objeto
{
    public string nombre;
    public float posX, posY, posZ;
    public float sizeX, sizeY, sizeZ;
    public float rotX, rotY, rotZ;
    public float forwardX, forwardY, forwardZ;
    public float masa;
    public string textura;
    public string mesh;

    public float colliderX, colliderY, colliderZ;
    public float colliderSizeX, colliderSizeY, colliderSizeZ;
    public string tag;
    public string extraTags;
    public bool hasChild;
    public float color_r;
    public float color_g;
    public float color_b;


    [SerializeField]
    public List<Objeto> o_children;
    [SerializeField]
    public List<Nodo> n_children;

    public Objeto()
    {
        n_children = new List<Nodo>();
        o_children = new List<Objeto>();
    }

    public string toJson()
    {
        string s=JsonUtility.ToJson(this);
        string modify = s.Substring(0,s.Length-1);
        // =s.Split('}')[0];
        int count=o_children.Count;
        int i = 0;
        modify += ",\"o_children\":[";
        foreach (Objeto child in o_children)
        {
            if (child.tag == "PhysicEntity")
            {
                modify += child.toJson();
                if (i < count - 1)
                    modify += ",";
                i++;
            }  
        }
        return modify+ "]}";
    }
    
}


[System.Serializable]
public class Nodo {
    public String name;
    public double posX, posY, posZ;
    public string extraInfo;
    [SerializeField]
    public List<Int32> conexiones;

    public  Nodo(String nombre,string tag) {
        name = nombre;
        extraInfo = tag;
        conexiones = new List<Int32>();
    }
}

public class Raiz
{

    public List<Objeto> children;

    public Raiz()
    {
        children = new List<Objeto>();
    }

    public void addChild(Objeto obj)
    {
        children.Add(obj);
    }

    public string toJson()
    {
        string json = "[";
        for (int i = 0; i < children.Count; i++)
        {
            string objeto = children.ElementAt(i).toJson() ;
            if (i < children.Count - 1)
            {
                json += objeto + ",";
            }
            else
            {
                json += objeto;
            }
        }
        json = json + "]";
        return json;
    }
}