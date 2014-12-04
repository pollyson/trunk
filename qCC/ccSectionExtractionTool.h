//##########################################################################
//#                                                                        #
//#                            CLOUDCOMPARE                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef CC_SECTION_EXTRACTION_TOOL_HEADER
#define CC_SECTION_EXTRACTION_TOOL_HEADER

//Local
#include <ccOverlayDialog.h>

//qCC_db
#include <ccHObject.h>

//Qt
#include <QList>

//GUI
#include <ui_sectionExtractionDlg.h>

class ccPolyline;
class ccGenericPointCloud;
class ccPointCloud;
class ccGLWindow;

//! Section extraction tool
class ccSectionExtractionTool : public ccOverlayDialog, public Ui::SectionExtractionDlg
{
	Q_OBJECT

public:

	//! Default constructor
	ccSectionExtractionTool(QWidget* parent);
	//! Destructor
	virtual ~ccSectionExtractionTool();

	//! Adds a cloud to the 'clouds' pool
	bool addCloud(ccGenericPointCloud* cloud, bool alreadyInDB = true);
	//! Adds a polyline to the 'sections' pool
	/** \warning: if this method returns true, the class takes the ownership of the cloud!
	**/
	bool addPolyline(ccPolyline* poly, bool alreadyInDB = true);
	
	//! Removes all registered entities (clouds & polylines)
	void removeAllEntities();
	
	//inherited from ccOverlayDialog
	virtual bool linkWith(ccGLWindow* win);
	virtual bool start();
	virtual void stop(bool accepted);

protected slots:

	void reset(bool askForConfirmation = true);
	void apply();
	void addPointToPolyline(int x, int y);
	void closePolyLine(int x=0, int y=0); //arguments for compatibility with ccGlWindow::rightButtonClicked signal
	void updatePolyLine(int x, int y, Qt::MouseButtons buttons);
	void enableSectionEditingMode(bool);
	void doImportPolylinesFromDB();
	void setVertDimension(int);

	//! To capture overridden shortcuts (pause button, etc.)
	void onShortcutTriggered(int);

protected:

	//! Projects a 2D (screen) point to 3D
	//CCVector3 project2Dto3D(int x, int y) const;

	//! Cancels current polyline edition
	void cancelCurrentPolyline();

	//! Imported entity
	template<class EntityType> struct ImportedEntity
	{
		//! Default constructor
		ImportedEntity()
			: entity(0)
			, originalDisplay(0)
			, isInDB(false)
		{}
		//! Copy constructor
		ImportedEntity(const ImportedEntity& section)
			: entity(section.entity)
			, originalDisplay(section.originalDisplay)
			, isInDB(section.isInDB)
		{}
		//! Constructor from a polyline
		ImportedEntity(EntityType* e, bool alreadyInDB)
			: entity(e)
			, originalDisplay(e->getDisplay())
			, isInDB(alreadyInDB)
		{}
		
		EntityType* entity;
		ccGenericGLDisplay* originalDisplay;
		bool isInDB;
	};

	//! Section
	typedef ImportedEntity<ccPolyline> Section;

	//! Cloud
	typedef ImportedEntity<ccGenericPointCloud> Cloud;

	//! Type of the pool of active sections
	typedef QList<Section> SectionPool;

	//! Pool of active sections
	SectionPool m_sections;

	//! Type of the pool of clouds
	typedef QList<Cloud> CloudPool;

	//! Pool of clouds
	CloudPool m_clouds;

	//! Process states
	enum ProcessStates
	{
		//...			= 1,
		//...			= 2,
		//...			= 4,
		//...			= 8,
		//...			= 16,
		PAUSED			= 32,
		STARTED			= 64,
		RUNNING			= 128,
	};

	//! Current process state
	unsigned m_state;

	//! Currently edited polyline
	ccPolyline* m_editedPoly;
	//! Segmentation polyline vertices
	ccPointCloud* m_editedPolyVertices;
};

#endif //CC_SECTION_EXTRACTION_TOOL_HEADER
