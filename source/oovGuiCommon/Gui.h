/*
 * Gui.h
 *
 *  Created on: Jul 10, 2013
 *  \copyright 2013 DCBlaha.  Distributed under the GPL.
 */

#ifndef GUI_H_
#define GUI_H_

#include "Builder.h"
#include "OovString.h"
#include <vector>


#define GUI_OK "_OK"
#define GUI_CANCEL "_Cancel"

class PathChooser
    {
    public:
	PathChooser():
	    mDialog(nullptr)
	    {}
	~PathChooser();
	void setDefaultPath(char const * const fn)
	    { mDefaultPath = fn; }
	bool ChoosePath(GtkWindow *parent, char const * const dlgName,
		GtkFileChooserAction action, std::string &path);
    private:
	GtkWidget *mDialog;
	std::string mDefaultPath;
    };

class Dialog
    {
    public:
	Dialog(GtkDialog *dlg=nullptr, GtkWindow *parent=nullptr):
	    mDialog(dlg)
	    {
	    setDialog(dlg, parent);
	    }
	virtual ~Dialog()
	    {}
	void setDialog(GtkDialog *dlg, GtkWindow *parent=nullptr)
	    {
	    mDialog = dlg;
	    if(parent)
		gtk_window_set_transient_for(GTK_WINDOW(dlg), parent);
	    }
	GtkWidget *addButton(const gchar *text, gint response_id)
	    { return gtk_dialog_add_button(mDialog, text, response_id); }
	int runHideCancel();
	// The response from this only works if the OK button Response ID in
	// Glade is set to -5 (GTK_RESPONSE_OK), and cancel is set to
	// -6 (GTK_RESPONSE_CANCEL).
	//
	// Use gtk_widget_hide_on_delete to prevent the close button from
	// destroying the dialog. In glade, this is set on the GtkDialog
	// top level window under the GtkWidget signals.
	bool run(bool hideDialogAfterButtonPress = false);
	virtual void beforeRun()
	    {}
	virtual void afterRun(bool ok)
	    {}
	GtkWidget *getContentArea()
	    { return gtk_dialog_get_content_area(mDialog); }
	void destroy()
	    { gtk_widget_destroy(GTK_WIDGET(mDialog)); }
	GtkDialog *getDialog()
	    { return mDialog; }

    private:
	GtkDialog *mDialog;
    };

class GuiText:public std::string
    {
    public:
	GuiText(void *text):
	    std::string(reinterpret_cast<char*>(text))
	    { g_free(text); }
    };

namespace Gui
    {
	inline GtkWindow *getWindow(GtkWidget *widget)
	    { return GTK_WINDOW(widget); }
	void clear(GtkTextView *textview);
	inline void clear(GtkComboBoxText *box)
	    { gtk_combo_box_text_remove_all(box); }
	void appendText(GtkTextView *textview, const std::string &text);
	inline void appendText(GtkComboBoxText *box, const std::string &text)
	    { gtk_combo_box_text_append_text(box, text.c_str()); }
	void scrollToCursor(GtkTextView *textview);
	inline void setText(GtkEntry *textentry, char const * const text)
	    { gtk_entry_set_text(textentry, text); }
	inline void setText(GtkLabel *label, char const * const text)
	    { gtk_label_set_text(label, text); }
	inline void setText(GtkTextView *view, char const * const text)
	    {
	    clear(view);
	    appendText(view, text);
	    }
	GuiText getText(GtkTextView *textview);
	inline char const * const getText(GtkEntry *entry)
	    { return gtk_entry_get_text(entry); }
	inline char const * const getText(GtkLabel *label)
	    { return gtk_label_get_text(label); }
	inline GuiText getText(GtkComboBoxText *cb)
	    { return GuiText(gtk_combo_box_text_get_active_text(cb)); }
	char const * const getSelectedText(GtkTextView *textview);
	int getCurrentLineNumber(GtkTextView *textView);
	GuiText getCurrentLineText(GtkTextView *textView);
	inline void setSelected(GtkComboBox *cb, int index)
	    { gtk_combo_box_set_active(cb, index); }
	inline void setSelected(GtkComboBoxText *cb, int index)
	    { gtk_combo_box_set_active(GTK_COMBO_BOX(cb), index); }

	inline void setEnabled(GtkButton *w, bool enabled)
	    { gtk_widget_set_sensitive(GTK_WIDGET(w), enabled); }
	inline void setEnabled(GtkLabel *w, bool enabled)
	    { gtk_widget_set_sensitive(GTK_WIDGET(w), enabled); }

	inline void setVisible(GtkWidget *w, bool show)
	    {
	    if(show)
		gtk_widget_show_all(GTK_WIDGET(w));
	    else
		gtk_widget_hide(GTK_WIDGET(w));
	    }
	inline void setVisible(GtkButton *w, bool show)
	    { setVisible(GTK_WIDGET(w), show); }
	inline void setVisible(GtkLabel *w, bool show)
	    { setVisible(GTK_WIDGET(w), show); }

	bool messageBox(char const * const msg,
		GtkMessageType msgType=GTK_MESSAGE_ERROR,
		GtkButtonsType buttons=GTK_BUTTONS_OK);
	inline int appendPage(GtkNotebook *notebook, GtkWidget *child,
            GtkWidget *tabLabel)
	    { return gtk_notebook_append_page(notebook, child, tabLabel); }
	inline int getCurrentPage(GtkNotebook *notebook)
	    { return gtk_notebook_get_current_page(notebook); }
	inline void setCurrentPage(GtkNotebook *notebook, int page)
	    { gtk_notebook_set_current_page(notebook, page); }
	int findTab(GtkNotebook *notebook, char const * const tabName);
	inline int getNumPages(GtkNotebook *notebook)
	    { return gtk_notebook_get_n_pages(notebook); }
	inline GtkWidget *getNthPage(GtkNotebook *notebook, int pageNum)
	    { return gtk_notebook_get_nth_page(notebook, pageNum); }
	void reparentWidget(GtkWidget *windowToMove, GtkContainer *newParent);
	inline void redraw(GtkWidget *widget)
	    { gtk_widget_queue_draw(widget); }
    };

class GuiTreeView
    {
    public:
	GuiTreeView():
	    mTreeView(nullptr)
	    {}
	void clear();
	void removeSelected();

    protected:
	GtkTreeView *mTreeView;
    };

class GuiList:public GuiTreeView
    {
    public:
	enum TreeViewItems
	 {
	   LIST_ITEM = 0,
	   N_COLUMNS
	 };
	void init(Builder &builder, char const * const widgetName,
		char const * const title);
	void appendText(char const * const str);
	void sort();
	std::vector<std::string> getText() const;
	std::string getSelected() const;
	int getSelectedIndex() const;
	void setSelected(char const * const str);
    };

class GuiTreeValue
    {
    public:
	GuiTreeValue():
	    mStr(nullptr)
	    {}
	~GuiTreeValue()
	    {
	    if(mStr)
		g_free(mStr);
	    }

    char *mStr;
    };

class GuiTreeItem
    {
    friend class GuiTree;
    public:
	GuiTreeItem(bool root = true):
	    mRoot(root)
	    {}
	bool isRoot() const
	    { return mRoot; }
	void setRoot(bool root=true)
	    { mRoot = true; }
	GtkTreeIter *getPtr()
	    { return(mRoot ? NULL : &mIter); }
    private:
	GtkTreeIter mIter;
	bool mRoot;
    };

/// This only supports a 2 level tree at this time.
class GuiTree:public GuiTreeView
    {
    public:
	enum TreeViewItems
            {
            LIST_ITEM = 0,
            N_COLUMNS
            };
	void init(Builder &builder, char const * const widgetName,
		char const * const title);
	/// Returns the newly created child item.
	GuiTreeItem appendText(GuiTreeItem parentItem, char const * const str);
	/// first element is parent, second is child
	std::vector<OovString> const getSelected() const;
	OovString const getSelected(char delimiter) const;
	void clearSelection()
	    { gtk_tree_selection_unselect_all(gtk_tree_view_get_selection(mTreeView)); }
	void setSelected(std::vector<OovString> const &names);
	void setSelected(OovString const &name, char delimeter);
	GuiTreeItem getItem(OovString const &name, char delimiter);

    private:
	// parent can be nullptr to find top level nodes,
	// otherwise this finds the child nodes.
	bool findNodeIter(GtkTreeIter *parent, OovString const &name,
		char delimiter, GtkTreeIter *childIter);
    };

class BackgroundDialog:public Dialog
    {
    public:
	BackgroundDialog();
	~BackgroundDialog();
	void setParentWindow(GtkWindow *parent)
	    { mParent = parent; }
	void setDialogText(char const *str);
	void setProgressIterations(int totalIters);
	// Return is false when a cancel is performed.
	bool updateProgressIteration(int currentIter);
	// Set from signal
	void cancelButtonPressed()
	    { mKeepGoing = false; }

    private:
	Builder *mBuilder;
	GtkWindow *mParent;
	bool mKeepGoing;
	int mTotalIters;
	time_t mStartTime;
	void showDialog(bool show);
    };

class RecursiveBackgroundLevel
    {
    public:
	RecursiveBackgroundLevel():
	    mLevel(0)
	    {}
	int mLevel;
    };

class RecursiveBackgroundDialog:public BackgroundDialog
    {
    public:
	RecursiveBackgroundDialog(RecursiveBackgroundLevel &level):
	    mLevel(level)
	    { ++mLevel.mLevel; }
	~RecursiveBackgroundDialog()
	    { --mLevel.mLevel; }
	void setDialogText(char const *str)
	    {
	    if(mLevel.mLevel == 1)
		BackgroundDialog::setDialogText(str);
	    }
	void setProgressIterations(int totalIters)
	    {
	    if(mLevel.mLevel == 1)
		BackgroundDialog::setProgressIterations(totalIters);
	    }
	bool updateProgressIteration(int currentIter)
	    {
	    if(mLevel.mLevel == 1)
		return BackgroundDialog::updateProgressIteration(currentIter);
	    else
		return true;
	    }

    private:
	RecursiveBackgroundLevel &mLevel;
    };

#endif /* GUI_H_ */
