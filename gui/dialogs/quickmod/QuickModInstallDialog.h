#pragma once

#include <QDialog>
#include <QMap>
#include <QPair>
#include <memory>

#include "logic/quickmod/QuickMod.h"
#include "logic/quickmod/QuickModVersion.h"

#include "logic/BaseInstance.h"

namespace Ui
{
class QuickModInstallDialog;
}

class QNetworkReply;
class WebDownloadNavigator;
class BaseInstance;
class OneSixInstance;
class QuickMod;
class QuickModInstaller;
class QTreeWidgetItem;

class QuickModInstallDialog : public QDialog
{
	Q_OBJECT

public:
	explicit QuickModInstallDialog(InstancePtr instance, QWidget *parent = 0);
	~QuickModInstallDialog();

public
slots:
	virtual int exec();

	void setInitialMods(const QList<QuickModPtr> mods);
	QList<QuickModVersionPtr> modVersions() const
	{
		return m_resolvedVersions;
	}

private
slots:
	void downloadNextMod();

	void urlCaught(QNetworkReply *reply);
	void processReply(QNetworkReply *reply, QuickModVersionPtr version);
	void downloadProgress(const qint64 current, const qint64 max);
	void downloadCompleted();


	/// Downloads dependency files.
	bool downloadDeps();

	/// Resolves dependencies.
	bool resolveDeps();

	/// Processes the version list. Installs any mods that are already downloaded.
	void processVersionList();

	/// Downloads all mods with direct download links.
	void runDirectDownloads();



	/// Downloads the given version as a webpage download.
	void runWebDownload(QuickModVersionPtr version);



	/** Checks if all downloads are finished and returns true if they are.
	 *  Note that this function also updates the state of the "finish" button.
	 */
	bool checkIsDone();

	/// Sets whether the web view is shown or not.
	void setWebViewShown(bool shown);


	/// Adds an entry to the progress list for the given version.
	void addProgressListEntry(QuickModVersionPtr version, const QString& url = tr("N/A"));

	/// Sets the progress list message for the given item.
	void setProgressListMsg(QuickModVersionPtr version, const QString& msg, const QColor& color = QColor(Qt::black));

	/// Sets whether the given progress item has its progress bar shown.
	void setShowProgressBar(QuickModVersionPtr version, bool show = true);

	/// Sets the progress bar value for the given version.
	void setVersionProgress(QuickModVersionPtr version, qint64 current, qint64 max);

	/// Sets the URL shown for the given version in the progress list.
	void setProgressListUrl(QuickModVersionPtr version, const QString& url);

	/// Gets the tree widget item for the given version.
	QTreeWidgetItem* itemForVersion(QuickModVersionPtr version) const;

private:
	Ui::QuickModInstallDialog *ui;
	QuickModInstaller *m_installer;

	InstancePtr m_instance;

	bool install(QuickModVersionPtr version);

	QList<QuickModPtr> m_initialMods;
	QList<QuickModVersionPtr> m_modVersions;
	QList<QuickModVersionPtr> m_resolvedVersions;
	QList<QUrl> m_downloadingUrls;

	/// A list of progress entries.
	/// The indices of the items in this list should correspond with the indices of each mod that's being installed.
	QMap<QuickModVersion*, QTreeWidgetItem*> m_progressEntries;
};
