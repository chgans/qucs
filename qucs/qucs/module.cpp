/***************************************************************************
                               module.cpp
                              ------------
    begin                : Thu Nov 5 2009
    copyright            : (C) 2009 by Stefan Jahn
    email                : stefan@lkcc.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <QHash>
#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>

#include "element.h"
#include "components/component.h"
#include "components/components.h"
#include "paintings/paintings.h"
#include "diagrams/diagrams.h"
#include "module.h"

// Global category and component lists.
QHash<QString, Module *> Module::Modules;
QList<Category *> Category::Categories;

QMap<QString, QString> Module::vaComponents;

// Constructor creates instance of module object.
Module::Module () {
  info = 0;
  category = "#special";
}

// Destructor removes instance of module object from memory.
Module::~Module () {
}

// Module registration using a category name and the appropriate
// function returning a modules instance object.
void Module::registerModule (QString category, pInfoFunc info) {
  Module * m = new Module ();
  m->info = info;
  m->category = category;
  intoCategory (m);
}

// Component registration using a category name and the appropriate
// function returning a components instance object.
void Module::registerComponent (QString category, pInfoFunc info) {
  Module * m = new Module ();
  m->info = info;
  m->category = category;

  // instantiation of the component once in order to obtain "Model"
  // property of the component
  QString Name, Model;
  char * File;
  Component * c = (Component *) info (Name, File, true);
  Model = c->Model;
  delete c;

  // put into category and the component hash
  intoCategory (m);
  if (!Modules.contains (Model))
    Modules.insert (Model, m);
}

// Returns instantiated component based on the given "Model" name.  If
// there is no such component registers the function returns NULL.
Component * Module::getComponent (QString Model) {
  if ( Modules.contains(Model)) {
    Module *m = Modules.find(Model).value();
    QString Name;
    char * File;
    QString vaBitmap;
    if (vaComponents.contains(Model))
      return (Component *)
              vacomponent::info (Name, vaBitmap, true, vaComponents[Model]);
    else
      return (Component *) m->info (Name, File, true);
  }
  return 0;
}

void Module::registerDynamicComponents()
{
    qDebug() << "Module::registerDynamicComponents()";


  // vaComponents is populated in QucsApp::slotLoadModule

  // register modules symbol and properties out of in vaComponents
  QMapIterator<QString, QString> i(vaComponents);
   while (i.hasNext()) {
     i.next();

//     qDebug() << i.key() << ": " << i.value() << endl;

     Module * m = new Module ();

     // what now? pointer to info?

     // the typedef needs to be different
     //passes the pointer, but it has no idea how to call the JSON
     m->infoVA = &vacomponent::info;

     // TODO maybe allow user load into custom category?
     m->category = QObject::tr("verilog-a user devices");

     // instantiation of the component once in order
     // to obtain "Model" property of the component
     //QString Name, Model;
     //char * File;
     QString Name, Model, vaBitmap;
//     char * File;
     Component * c = (Component *)
             vacomponent::info (Name, vaBitmap, true, vaComponents[i.key()]);
     Model = c->Model;
     delete c;

     // put into category and the component hash
     intoCategory (m);

     if (!Modules.contains (Model))
         Modules.insert (Model, m);

   } // while
}

// The function appends the given module to the appropriate category.
// If there is no such category yet, then the category gets created.
void Module::intoCategory (Module * m) {

  // look through existing categories
  QList<Category *>::const_iterator it;
  for (it = Category::Categories.constBegin();
       it != Category::Categories.constEnd(); it++) {
    if ((*it)->Name == m->category) {
      (*it)->Content.append (m);
      break;
    }
  }

  // if there is no such category, then create it
  if (it == Category::Categories.constEnd()) {
    Category *cat = new Category (m->category);
    Category::Categories.append (cat);
    cat->Content.append (m);
  }
}

// Helper macros for module registration.
#define REGISTER_MOD_1(cat,val) \
  registerModule (cat, &val::info)
#define REGISTER_MOD_2(cat,val,inf1,inf2) \
  registerModule (cat, &val::inf1); \
  registerModule (cat, &val::inf2)
#define REGISTER_MOD_3(cat,val,inf1,inf2,inf3) \
  registerModule (cat, &val::inf1); \
  registerModule (cat, &val::inf2); \
  registerModule (cat, &val::inf3)

#define REGISTER_COMP_1(cat,val) \
  registerComponent (cat, &val::info)
#define REGISTER_COMP_2(cat,val,inf1,inf2) \
  registerComponent (cat, &val::inf1); \
  registerComponent (cat, &val::inf2)
#define REGISTER_COMP_3(cat,val,inf1,inf2,inf3) \
  registerComponent (cat, &val::inf1); \
  registerComponent (cat, &val::inf2); \
  registerComponent (cat, &val::inf3)

#define REGISTER_LUMPED_1(val) \
  REGISTER_COMP_1 (QObject::tr("lumped components"),val)
#define REGISTER_LUMPED_2(val,inf1,inf2) \
  REGISTER_COMP_2 (QObject::tr("lumped components"),val,inf1,inf2)
#define REGISTER_SOURCE_1(val) \
  REGISTER_COMP_1 (QObject::tr("sources"),val)
#define REGISTER_PROBE_1(val) \
  REGISTER_COMP_1 (QObject::tr("probes"),val)
#define REGISTER_TRANS_1(val) \
  REGISTER_COMP_1 (QObject::tr("transmission lines"),val)
#define REGISTER_NONLINEAR_1(val) \
  REGISTER_COMP_1 (QObject::tr("nonlinear components"),val)
#define REGISTER_NONLINEAR_2(val,inf1,inf2) \
  REGISTER_COMP_2 (QObject::tr("nonlinear components"),val,inf1,inf2)
#define REGISTER_NONLINEAR_3(val,inf1,inf2,inf3) \
  REGISTER_COMP_3 (QObject::tr("nonlinear components"),val,inf1,inf2,inf3)
#define REGISTER_VERILOGA_1(val) \
  REGISTER_COMP_1 (QObject::tr("verilog-a devices"),val)
#define REGISTER_VERILOGA_2(val,inf1,inf2) \
  REGISTER_COMP_2 (QObject::tr("verilog-a devices"),val,inf1,inf2)
#define REGISTER_DIGITAL_1(val) \
  REGISTER_COMP_1 (QObject::tr("digital components"),val)
#define REGISTER_FILE_1(val) \
  REGISTER_COMP_1 (QObject::tr("file components"),val)
#define REGISTER_FILE_3(val,inf1,inf2,inf3) \
  REGISTER_COMP_3 (QObject::tr("file components"),val,inf1,inf2,inf3)
#define REGISTER_SIMULATION_1(val) \
  REGISTER_COMP_1 (QObject::tr("simulations"),val)
#define REGISTER_DIAGRAM_1(val) \
  REGISTER_MOD_1 (QObject::tr("diagrams"),val)
#define REGISTER_DIAGRAM_2(val,inf1,inf2) \
  REGISTER_MOD_2 (QObject::tr("diagrams"),val,inf1,inf2)
#define REGISTER_PAINT_1(val) \
  REGISTER_MOD_1 (QObject::tr("paintings"),val)
#define REGISTER_PAINT_2(val,inf1,inf2) \
  REGISTER_MOD_2 (QObject::tr("paintings"),val,inf1,inf2)
#define REGISTER_EXTERNAL_1(val) \
  REGISTER_COMP_1 (QObject::tr("external sim components"),val)
#define REGISTER_SPICE_1(val) \
  REGISTER_COMP_1 (QObject::tr("spice components"),val)
#define REGISTER_SPICE_SEC_1(val) \
  REGISTER_COMP_1 (QObject::tr("spice specific sections"),val)
#define REGISTER_SPICE_SIM_1(val) \
  REGISTER_COMP_1 (QObject::tr("spice simulations"),val)

// This function has to be called once at application startup.  It
// registers every component available in the application.  Put here
// any new component.
void Module::registerModules (void) {

  // lumped components
  REGISTER_LUMPED_2 (Resistor, info, info_us);
  REGISTER_LUMPED_1 (Capacitor);
  REGISTER_LUMPED_1 (Inductor);
  REGISTER_LUMPED_1 (Ground);
  REGISTER_LUMPED_1 (SubCirPort);
  REGISTER_LUMPED_1 (Transformer);
  REGISTER_LUMPED_1 (symTrafo);
  REGISTER_LUMPED_1 (dcBlock);
  REGISTER_LUMPED_1 (dcFeed);
  REGISTER_LUMPED_1 (BiasT);
  REGISTER_LUMPED_1 (Attenuator);
  REGISTER_LUMPED_1 (Amplifier);
  REGISTER_LUMPED_1 (Isolator);
  REGISTER_LUMPED_1 (Circulator);
  REGISTER_LUMPED_1 (Gyrator);
  REGISTER_LUMPED_1 (Phaseshifter);
  REGISTER_LUMPED_1 (Coupler);
  REGISTER_LUMPED_1 (Hybrid);
  REGISTER_LUMPED_1 (iProbe);
  REGISTER_LUMPED_1 (vProbe);
  REGISTER_LUMPED_1 (Mutual);
  REGISTER_LUMPED_1 (Mutual2);
  REGISTER_LUMPED_1 (MutualX);
  REGISTER_LUMPED_1 (Switch);
  REGISTER_LUMPED_1 (Relais);
  REGISTER_LUMPED_1 (RFedd);
  REGISTER_LUMPED_1 (RFedd2P);

  // sources
  REGISTER_SOURCE_1 (Volt_dc);
  REGISTER_SOURCE_1 (Ampere_dc);
  REGISTER_SOURCE_1 (Volt_ac);
  REGISTER_SOURCE_1 (Ampere_ac);
  REGISTER_SOURCE_1 (Source_ac);
  REGISTER_SOURCE_1 (Volt_noise);
  REGISTER_SOURCE_1 (Ampere_noise);
  REGISTER_SOURCE_1 (VCCS);
  REGISTER_SOURCE_1 (CCCS);
  REGISTER_SOURCE_1 (VCVS);
  REGISTER_SOURCE_1 (CCVS);
  REGISTER_SOURCE_1 (vPulse);
  REGISTER_SOURCE_1 (iPulse);
  REGISTER_SOURCE_1 (vRect);
  REGISTER_SOURCE_1 (iRect);
  REGISTER_SOURCE_1 (Noise_ii);
  REGISTER_SOURCE_1 (Noise_vv);
  REGISTER_SOURCE_1 (Noise_iv);
  REGISTER_SOURCE_1 (AM_Modulator);
  REGISTER_SOURCE_1 (PM_Modulator);
  REGISTER_SOURCE_1 (iExp);
  REGISTER_SOURCE_1 (vExp);
  REGISTER_SOURCE_1 (vFile);
  REGISTER_SOURCE_1 (iFile);


  // probes
  REGISTER_PROBE_1 (iProbe);
  REGISTER_PROBE_1 (vProbe);
  REGISTER_PROBE_1 (Cmeter_SPICE);

  // transmission lines
  REGISTER_TRANS_1 (TLine);
  REGISTER_TRANS_1 (TLine_4Port);
  REGISTER_TRANS_1 (CoupledTLine);
  REGISTER_TRANS_1 (TwistedPair);
  REGISTER_TRANS_1 (CoaxialLine);
  REGISTER_TRANS_1 (RectLine);
  REGISTER_TRANS_1 (RLCG);
  REGISTER_TRANS_1 (Substrate);
  REGISTER_TRANS_1 (MSline);
  REGISTER_TRANS_1 (MScoupled);
  REGISTER_TRANS_1 (MSlange);
  REGISTER_TRANS_1 (MScorner);
  REGISTER_TRANS_1 (MSmbend);
  REGISTER_TRANS_1 (MSstep);
  REGISTER_TRANS_1 (MStee);
  REGISTER_TRANS_1 (MScross);
  REGISTER_TRANS_1 (MSopen);
  REGISTER_TRANS_1 (MSgap);
  REGISTER_TRANS_1 (MSvia);
  REGISTER_TRANS_1 (MSrstub);
  REGISTER_TRANS_1 (Coplanar);
  REGISTER_TRANS_1 (CPWopen);
  REGISTER_TRANS_1 (CPWshort);
  REGISTER_TRANS_1 (CPWgap);
  REGISTER_TRANS_1 (CPWstep);
  REGISTER_TRANS_1 (BondWire);

  // nonlinear components
  REGISTER_NONLINEAR_1 (Diode);
  REGISTER_NONLINEAR_2 (BJT, info, info_pnp);
  REGISTER_NONLINEAR_2 (BJTsub, info, info_pnp);
  REGISTER_NONLINEAR_2 (JFET, info, info_p);
  REGISTER_NONLINEAR_3 (MOSFET, info, info_p, info_depl);
  REGISTER_NONLINEAR_3 (MOSFET_sub, info, info_p, info_depl);
  REGISTER_NONLINEAR_1 (OpAmp);
  REGISTER_NONLINEAR_1 (EqnDefined);
  REGISTER_NONLINEAR_1 (Diac);
  REGISTER_NONLINEAR_1 (Triac);
  REGISTER_NONLINEAR_1 (Thyristor);
  REGISTER_NONLINEAR_1 (TunnelDiode);

  // verilog-a devices
  REGISTER_VERILOGA_1 (hicumL2V2p1);
  REGISTER_VERILOGA_1 (HBT_X);
  REGISTER_VERILOGA_1 (mod_amp);
  REGISTER_VERILOGA_1 (hic2_full);
  REGISTER_VERILOGA_1 (log_amp);
  REGISTER_VERILOGA_2 (hic0_full, info, info_pnp);
  REGISTER_VERILOGA_1 (potentiometer);
  REGISTER_VERILOGA_1 (MESFET);
  REGISTER_VERILOGA_2 (EKV26MOS, info, info_pmos);
  REGISTER_VERILOGA_1 (bsim3v34nMOS);
  REGISTER_VERILOGA_1 (bsim3v34pMOS);
  REGISTER_VERILOGA_1 (bsim4v30nMOS);
  REGISTER_VERILOGA_1 (bsim4v30pMOS);
  REGISTER_VERILOGA_2 (hicumL0V1p2, info, info_pnp);
  REGISTER_VERILOGA_2 (hicumL0V1p2g, info, info_pnp);
  REGISTER_VERILOGA_2 (hicumL0V1p3, info, info_pnp);
  REGISTER_VERILOGA_1 (hicumL2V2p23);
  REGISTER_VERILOGA_1 (hicumL2V2p24);
  REGISTER_VERILOGA_1 (hicumL2V2p31n);
  REGISTER_VERILOGA_1 (photodiode);
  REGISTER_VERILOGA_1 (phototransistor);
  REGISTER_VERILOGA_1 (nigbt);
  REGISTER_VERILOGA_1 (vcresistor);

  // digital components
  REGISTER_DIGITAL_1 (Digi_Source);
  REGISTER_DIGITAL_1 (Logical_Inv);
  REGISTER_DIGITAL_1 (Logical_OR);
  REGISTER_DIGITAL_1 (Logical_NOR);
  REGISTER_DIGITAL_1 (Logical_AND);
  REGISTER_DIGITAL_1 (Logical_NAND);
  REGISTER_DIGITAL_1 (Logical_XOR);
  REGISTER_DIGITAL_1 (Logical_XNOR);
  REGISTER_DIGITAL_1 (Logical_Buf);
  REGISTER_DIGITAL_1 (andor4x2);
  REGISTER_DIGITAL_1 (andor4x3);
  REGISTER_DIGITAL_1 (andor4x4);
  REGISTER_DIGITAL_1 (mux2to1);
  REGISTER_DIGITAL_1 (mux4to1);
  REGISTER_DIGITAL_1 (mux8to1);
  REGISTER_DIGITAL_1 (dmux2to4);
  REGISTER_DIGITAL_1 (dmux3to8);
  REGISTER_DIGITAL_1 (dmux4to16);
  REGISTER_DIGITAL_1 (ha1b);
  REGISTER_DIGITAL_1 (fa1b);
  REGISTER_DIGITAL_1 (fa2b);
  REGISTER_DIGITAL_1 (RS_FlipFlop);
  REGISTER_DIGITAL_1 (D_FlipFlop);
  REGISTER_DIGITAL_1 (dff_SR);
  REGISTER_DIGITAL_1 (JK_FlipFlop);
  REGISTER_DIGITAL_1 (jkff_SR);
  REGISTER_DIGITAL_1 (tff_SR);
  REGISTER_DIGITAL_1 (gatedDlatch);
  REGISTER_DIGITAL_1 (logic_0);
  REGISTER_DIGITAL_1 (logic_1);
  REGISTER_DIGITAL_1 (pad2bit);
  REGISTER_DIGITAL_1 (pad3bit);
  REGISTER_DIGITAL_1 (pad4bit);
  REGISTER_DIGITAL_1 (DLS_nto1);
  REGISTER_DIGITAL_1 (DLS_1ton);
  REGISTER_DIGITAL_1 (binarytogrey4bit);
  REGISTER_DIGITAL_1 (greytobinary4bit);
  REGISTER_DIGITAL_1 (comp_1bit);
  REGISTER_DIGITAL_1 (comp_2bit);
  REGISTER_DIGITAL_1 (comp_4bit);
  REGISTER_DIGITAL_1 (hpribin4bit);
  REGISTER_DIGITAL_1 (VHDL_File);
  REGISTER_DIGITAL_1 (Verilog_File);
  REGISTER_DIGITAL_1 (Digi_Sim);

  // file components
  REGISTER_FILE_1 (SpiceFile);
  REGISTER_FILE_3 (SParamFile, info1, info2, info);
  REGISTER_FILE_1 (Subcircuit);

  // simulations
  REGISTER_SIMULATION_1 (DC_Sim);
  REGISTER_SIMULATION_1 (TR_Sim);
  REGISTER_SIMULATION_1 (AC_Sim);
  REGISTER_SIMULATION_1 (SP_Sim);
  REGISTER_SIMULATION_1 (HB_Sim);
  REGISTER_SIMULATION_1 (Param_Sweep);
  REGISTER_SIMULATION_1 (Digi_Sim);
  REGISTER_SIMULATION_1 (Optimize_Sim);

  // diagrams
  REGISTER_DIAGRAM_1 (RectDiagram);
  REGISTER_DIAGRAM_1 (PolarDiagram);
  REGISTER_DIAGRAM_1 (TabDiagram);
  REGISTER_DIAGRAM_2 (SmithDiagram, info, info_y);
  REGISTER_DIAGRAM_2 (PSDiagram, info, info_sp);
  REGISTER_DIAGRAM_1 (Rect3DDiagram);
  REGISTER_DIAGRAM_1 (CurveDiagram);
  REGISTER_DIAGRAM_1 (TimingDiagram);
  REGISTER_DIAGRAM_1 (TruthDiagram);

  // external simulation
  REGISTER_EXTERNAL_1 (ETR_Sim);
  REGISTER_EXTERNAL_1 (ecvs);

  //spice specific components
  REGISTER_SPICE_1 (Src_eqndef);
  REGISTER_SPICE_1 (iSffm);
  REGISTER_SPICE_1 (vAmpMod);
  REGISTER_SPICE_1 (iAmpMod); 
  REGISTER_SPICE_1 (vSffm);
  REGISTER_SPICE_1 (vPWL); 
  REGISTER_SPICE_1 (iPWL);   
  REGISTER_SPICE_1 (eNL);   
  REGISTER_SPICE_1 (gNL);  
  REGISTER_SPICE_1 (vTRNOISE);   
  REGISTER_SPICE_1 (iTRNOISE);   
  REGISTER_SPICE_1 (vTRRANDOM); 
  REGISTER_SPICE_1 (C_SPICE); 
  REGISTER_SPICE_1 (L_SPICE); 
  REGISTER_SPICE_1 (R_SPICE);  
  REGISTER_SPICE_1 (K_SPICE);  
  REGISTER_SPICE_1 (MESFET_SPICE);  
  REGISTER_SPICE_1 (LTL_SPICE); 
  REGISTER_SPICE_1 (UDRCTL_SPICE);      

  // specific sections of spice netlists
  REGISTER_SPICE_SEC_1 (SpiceParam);
  REGISTER_SPICE_SEC_1 (SpiceGlobalParam);
  REGISTER_SPICE_SEC_1 (SpiceOptions);

  // spice simulations
  REGISTER_SPICE_SIM_1 (SpiceFourier);
  REGISTER_SPICE_SIM_1 (SpiceCustomSim);


  // paintings
  REGISTER_PAINT_1 (GraphicLine);
  REGISTER_PAINT_1 (Arrow);
  REGISTER_PAINT_1 (GraphicText);
  REGISTER_PAINT_2 (Ellipse, info, info_filled);
  REGISTER_PAINT_2 (Rectangle, info, info_filled);
  REGISTER_PAINT_1 (EllipseArc);

}

// This function has to be called once at application end.  It removes
// all categories and registered modules from memory.
void Module::unregisterModules (void) {
  while(!Category::Categories.isEmpty()) {
    delete Category::Categories.takeFirst();
  }

  //remove all modules by iterator, require in qhash
  QHashIterator<QString, Module *> it( Modules );
  while(it.hasNext()) {
    it.next();
    delete it.value();
  }
  Modules.clear ();
}

// Constructor creates instance of module object.
Category::Category () {
  Name = "#special";
  while(!Content.isEmpty()) {
    delete Content.takeFirst();
  }
}

// Constructor creates named instance of module object.
Category::Category (const QString name) {
  Name = name;
  while(!Content.isEmpty()) {
    delete Content.takeFirst();
  }
}

// Destructor removes instance of module object from memory.
Category::~Category () {
  while(!Content.isEmpty()) {
    delete Content.takeFirst();
  }
}

// Returns the available category names in a list of strings.
QStringList Category::getCategories (void) {
  QStringList res;
  QList<Category *>::const_iterator it;
  for (it = Category::Categories.constBegin(); 
       it != Category::Categories.constEnd(); it++) {
    res.append ((*it)->Name);
  }
  return res;
}

// The function returns the registered modules in the given category
// as a pointer list.  The pointer list is empty if there is no such
// category available.
QList<Module *> Category::getModules (QString category) {
  QList<Module *> res;
  QList<Category *>::const_iterator it;
  for (it = Category::Categories.constBegin();
       it != Category::Categories.constEnd(); it++) {
    if (category == (*it)->Name) {
      res = (*it)->Content;
    }
  }
  return res;
}

// Returns the index number into the category list for the given
// category name.  The function returns minus 1 if there is no such
// category.
int Category::getModulesNr (QString category) {
  for (int i = 0; i < Category::Categories.size(); i++) {
    if (category == Category::Categories.at(i)->Name)
      return i;
  }
  return -1;
}
