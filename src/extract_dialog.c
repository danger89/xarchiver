/*
 *  Copyright (C) 2008 Giuseppe Torelli - <colossus73@gmail.com>
 *  Copyright (C) 2006 Benedikt Meurer - <benny@xfce.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Street #330, Boston, MA 02111-1307, USA.
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include "extract_dialog.h"
#include "interface.h"
#include "window.h"
#include "string_utils.h"
#include "support.h"

gboolean stop_flag;
extern gboolean unrar;
gchar *rar;

Extract_dialog_data *xa_create_extract_dialog (gint selected,XArchive *archive)
{
	GSList *radiobutton1_group = NULL;
	Extract_dialog_data *dialog_data;
	stop_flag = FALSE;

	home_dir = g_get_home_dir();
	dialog_data = g_new0 (Extract_dialog_data, 1);
	dialog_data->dialog1 = gtk_dialog_new ();
	if (archive->type == XARCHIVETYPE_BZIP2 || archive->type == XARCHIVETYPE_GZIP || archive->type == XARCHIVETYPE_LZMA)
		gtk_window_set_title (GTK_WINDOW (dialog_data->dialog1), _("Decompress file"));
	else
		gtk_window_set_title (GTK_WINDOW (dialog_data->dialog1), _("Extract files from archive"));
	gtk_window_set_type_hint (GTK_WINDOW (dialog_data->dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_transient_for (GTK_WINDOW(dialog_data->dialog1),GTK_WINDOW (MainWindow));
	gtk_dialog_set_has_separator (GTK_DIALOG(dialog_data->dialog1),FALSE);
	gtk_widget_set_size_request (dialog_data->dialog1,492,372);

	option_tooltip = gtk_tooltips_new ();
	dialog_data->dialog_vbox1 = GTK_DIALOG (dialog_data->dialog1)->vbox;

	vbox1 = gtk_vbox_new (FALSE, 2);
	gtk_box_pack_start (GTK_BOX (dialog_data->dialog_vbox1), vbox1, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox1), 2);

	label1 = gtk_label_new (_("Extract to:"));
	gtk_box_pack_start (GTK_BOX (vbox1), label1, FALSE, FALSE, 0);
	gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);

	dialog_data->destination_path_entry = gtk_entry_new ();
	gtk_entry_set_text (GTK_ENTRY(dialog_data->destination_path_entry),home_dir);
	gtk_box_pack_start (GTK_BOX (vbox1), dialog_data->destination_path_entry, FALSE, FALSE, 0);

	hbox1 = gtk_hbox_new (TRUE, 10);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

	vbox2 = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox2, TRUE, TRUE, 0);

	frame1 = gtk_frame_new (NULL);
	gtk_box_pack_start (GTK_BOX (vbox2), frame1, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_OUT);

	alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame1), alignment1);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 0, 0, 12, 0);

	vbox3 = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (alignment1), vbox3);

	dialog_data->all_files_radio = gtk_radio_button_new_with_mnemonic (NULL, _("All files"));
	gtk_box_pack_start (GTK_BOX (vbox3), dialog_data->all_files_radio, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (dialog_data->all_files_radio), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialog_data->all_files_radio));

	radiobutton1 = gtk_radio_button_new_with_mnemonic (NULL, _("Selected files"));
	gtk_box_pack_start (GTK_BOX (vbox3), radiobutton1, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton1), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton1));

	if (selected)
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton1),TRUE);
	else
		gtk_widget_set_sensitive (radiobutton1,FALSE);

	hbox2 = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox3), hbox2, FALSE, FALSE, 0);

	radiobutton3 = gtk_radio_button_new_with_mnemonic (NULL, _("Files: "));
	gtk_box_pack_start (GTK_BOX (hbox2), radiobutton3, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton3), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton3));

	entry2 = gtk_entry_new ();
	gtk_box_pack_start (GTK_BOX (hbox2), entry2, TRUE, TRUE, 0);
	gtk_entry_set_width_chars (GTK_ENTRY (entry2),10);

	label2 = gtk_label_new (_("Files "));
	gtk_frame_set_label_widget (GTK_FRAME (frame1), label2);

	frame2 = gtk_frame_new (NULL);
	gtk_box_pack_start (GTK_BOX (vbox2), frame2, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_OUT);

	alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame2), alignment2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);
	
	vbox5 = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (alignment2), vbox5);
	
	dialog_data->overwrite_check = gtk_check_button_new_with_mnemonic (_("Overwrite existing files"));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog_data->overwrite_check), archive->overwrite);
	gtk_box_pack_start (GTK_BOX (vbox5), dialog_data->overwrite_check, FALSE, FALSE, 0);

	dialog_data->extract_full = gtk_check_button_new_with_mnemonic (_("Extract files with full path"));
	/*if (archive->type == XARCHIVETYPE_GZIP || archive->type == XARCHIVETYPE_LZMA || archive->type == XARCHIVETYPE_BZIP2 )
		goto here;*/
	if (archive->type == XARCHIVETYPE_TAR || archive->type == XARCHIVETYPE_TAR_GZ || archive->type == XARCHIVETYPE_TAR_LZMA || archive->type == XARCHIVETYPE_DEB || archive->type == XARCHIVETYPE_TAR_BZ2)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog_data->extract_full), TRUE);
		//gtk_widget_set_sensitive (dialog_data->extract_full, FALSE);
	}
	else
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog_data->extract_full), archive->full_path);

	gtk_tooltips_set_tip(option_tooltip,dialog_data->extract_full , _("The archive's directory structure is recreated in the extraction directory."), NULL );
	gtk_box_pack_start (GTK_BOX (vbox5), dialog_data->extract_full, FALSE, FALSE, 0);

	if (archive->type == XARCHIVETYPE_TAR || archive->type == XARCHIVETYPE_TAR_GZ || archive->type == XARCHIVETYPE_TAR_LZMA || archive->type == XARCHIVETYPE_TAR_BZ2 || archive->type == XARCHIVETYPE_DEB)
	{
		dialog_data->touch = gtk_check_button_new_with_mnemonic (_("Touch files"));
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog_data->touch), archive->tar_touch);
		gtk_tooltips_set_tip (option_tooltip,dialog_data->touch, _("When this option is used, tar leaves the data modification times of the files it extracts as the times when the files were extracted, instead of setting it to the times recorded in the archive."), NULL );
		gtk_box_pack_start (GTK_BOX (vbox5), dialog_data->touch, FALSE, FALSE, 0);
/*here:
		dialog_data->hbox6 = gtk_hbox_new (FALSE, 2);
		gtk_widget_show (dialog_data->hbox6);
		gtk_box_pack_start (GTK_BOX (vbox5), dialog_data->hbox6, FALSE, FALSE, 0);*/
	}
	else
		dialog_data->touch = NULL;

	if (archive->type == XARCHIVETYPE_RAR || archive->type == XARCHIVETYPE_ZIP || archive->type == XARCHIVETYPE_ARJ)
	{
		dialog_data->fresh = gtk_check_button_new_with_mnemonic (_("Freshen existing files"));
		gtk_tooltips_set_tip (option_tooltip,dialog_data->fresh , _("Extract only those files that already exist on disk and that are newer than the disk copies."), NULL );
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog_data->fresh), archive->freshen);
		gtk_box_pack_start (GTK_BOX (vbox5), dialog_data->fresh, FALSE, FALSE, 0);
		g_signal_connect (G_OBJECT (dialog_data->fresh),"toggled",G_CALLBACK (fresh_update_toggled_cb) , dialog_data);

		dialog_data->update = gtk_check_button_new_with_mnemonic (_("Update existing files"));
		gtk_tooltips_set_tip (option_tooltip,dialog_data->update , _("This option performs the same function as the freshen one, extracting files that are newer than those with the same name on disk, and in addition it extracts those files that do not already exist on disk."), NULL );
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog_data->update), archive->update);
		gtk_box_pack_start (GTK_BOX (vbox5), dialog_data->update, FALSE, FALSE, 0);
		g_signal_connect (G_OBJECT (dialog_data->update),"toggled",G_CALLBACK (update_fresh_toggled_cb) , dialog_data);
	}
	else
	{
		dialog_data->fresh = NULL;
		dialog_data->update = NULL;
	}

	label3 = gtk_label_new (_("Options "));
	gtk_frame_set_label_widget (GTK_FRAME (frame2), label3);

	vbox4 = gtk_vbox_new (FALSE, 4);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox4, TRUE, TRUE, 0);

	label4 = gtk_label_new (_("Directories Tree:"));
	gtk_box_pack_start (GTK_BOX (vbox4), label4, FALSE, FALSE, 0);
	gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);

	scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start (GTK_BOX (vbox4), scrolledwindow1, TRUE, TRUE, 0);
	g_object_set (G_OBJECT (scrolledwindow1),"hscrollbar-policy", GTK_POLICY_AUTOMATIC,"shadow-type", GTK_SHADOW_IN,"vscrollbar-policy", GTK_POLICY_AUTOMATIC, NULL);

	model = gtk_tree_store_new (3,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	treeview3 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
	gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview3);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview3), FALSE);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(model),1,GTK_SORT_ASCENDING);
	g_signal_connect (G_OBJECT (treeview3),"row-expanded",G_CALLBACK(xa_expand_dir),dialog_data->destination_path_entry);
	GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW (treeview3));
	g_signal_connect ((gpointer) sel,"changed",G_CALLBACK (xa_tree_view_row_selected),dialog_data->destination_path_entry);

	column = gtk_tree_view_column_new();
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column,renderer, FALSE);
	gtk_tree_view_column_set_attributes(column,renderer, "stock-id",0,NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start(column,renderer, TRUE);
	gtk_tree_view_column_set_attributes( column,renderer,"text",1,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview3), column);

	alignment3 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_box_pack_start (GTK_BOX (vbox1), alignment3, FALSE, FALSE, 0);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment3), 0, 0, 370, 0);
  
	create_dir = gtk_button_new_with_mnemonic (_("Create Dir"));
	gtk_container_add (GTK_CONTAINER (alignment3),create_dir);
  
	hbox3 = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox5), hbox3, TRUE, TRUE, 0);

	label_password = gtk_label_new (_("Password:"));
	gtk_box_pack_start (GTK_BOX (hbox3), label_password, FALSE, FALSE, 0);

	dialog_data->password_entry = gtk_entry_new ();
	gtk_box_pack_start (GTK_BOX (hbox3), dialog_data->password_entry, TRUE, TRUE, 0);
	gtk_entry_set_visibility (GTK_ENTRY (dialog_data->password_entry), FALSE);
	if (archive->has_passwd)
    {
		gtk_widget_set_sensitive (label_password, TRUE);
		gtk_widget_set_sensitive (dialog_data->password_entry, TRUE);
		if (archive->passwd != NULL)
			gtk_entry_set_text (GTK_ENTRY(dialog_data->password_entry),archive->passwd);
    }
	else
	{
		gtk_widget_set_sensitive (label_password,FALSE);
		gtk_widget_set_sensitive (dialog_data->password_entry,FALSE);
	}

	dialog_action_area1 = GTK_DIALOG (dialog_data->dialog1)->action_area;
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

	cancel_button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog_data->dialog1),cancel_button, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

	extract_button = gtk_button_new();
	extract_image = xa_main_window_find_image("xarchiver-extract.png", GTK_ICON_SIZE_SMALL_TOOLBAR);
	extract_hbox = gtk_hbox_new(FALSE, 4);
	extract_label = gtk_label_new_with_mnemonic(_("_Extract"));

	alignment3 = gtk_alignment_new (0.5, 0.5, 0, 0);
	gtk_container_add (GTK_CONTAINER (alignment3),extract_hbox);
	gtk_box_pack_start(GTK_BOX(extract_hbox),extract_image, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(extract_hbox),extract_label, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(extract_button),alignment3);

	gtk_dialog_add_action_widget (GTK_DIALOG (dialog_data->dialog1), extract_button, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (extract_button, GTK_CAN_DEFAULT);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog_data->dialog1), GTK_RESPONSE_OK);
	xa_browse_dir(model,"/",NULL);
	gtk_widget_show_all(dialog_data->dialog1);
	return dialog_data;
}

void fresh_update_toggled_cb (GtkToggleButton *button, Extract_dialog_data *data)
{
	gboolean active = gtk_toggle_button_get_active (button);
	if (active)
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data->update), FALSE);
}

void update_fresh_toggled_cb (GtkToggleButton *button, Extract_dialog_data *data)
{
	if (data->fresh == NULL)
		return;
	gboolean active = gtk_toggle_button_get_active (button);
	if (active)
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data->fresh),FALSE);
}

gchar *xa_parse_extract_dialog_options (XArchive *archive,Extract_dialog_data *dialog_data,GtkTreeSelection *selection)
{
	gchar *command = NULL;
	gchar *tar;
	gchar *destination_path = NULL;
	gboolean done = FALSE;
	GString *names;

	if (unrar)
		rar = "unrar";
	else
		rar = "rar";

    while (! done)
	{
		switch (gtk_dialog_run(GTK_DIALOG(dialog_data->dialog1)))
		{
			case GTK_RESPONSE_CANCEL:
			case GTK_RESPONSE_DELETE_EVENT:
			done = TRUE;
			if (MainWindow && (archive->type == XARCHIVETYPE_GZIP || archive->type == XARCHIVETYPE_LZMA || archive->type == XARCHIVETYPE_BZIP2) )
			{
				gtk_widget_set_sensitive (Stop_button,FALSE);
				Update_StatusBar (_("Operation canceled."));
				gtk_widget_hide (viewport2);
				xa_set_button_state (1,1,GTK_WIDGET_IS_SENSITIVE(close1),0,0,0,0,0);
				archive->status = XA_ARCHIVESTATUS_IDLE;
			}
			break;

			case GTK_RESPONSE_OK:
			destination_path = g_strdup (gtk_entry_get_text (GTK_ENTRY (dialog_data->destination_path_entry)));
			archive->extraction_path = xa_escape_bad_chars (destination_path , "$\'`\"\\!?* ()&|@#:;");

			if (strlen(archive->extraction_path) == 0)
			{
				response = xa_show_message_dialog (GTK_WINDOW (MainWindow),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK, _("You missed where to extract the files!"),_("Please enter the extraction path.") );
				break;
			}
			if (archive->extraction_path[0] != '/')
			{
				gchar *cur_dir = g_get_current_dir();
				archive->extraction_path = g_strconcat(cur_dir, "/",archive->extraction_path,NULL);
				g_free (cur_dir);
			}
			if (archive->has_passwd)
				archive->passwd  = g_strdup (gtk_entry_get_text (GTK_ENTRY(dialog_data->password_entry)));

			if (archive->has_passwd && strlen(archive->passwd) == 0 )
			{
				response = xa_show_message_dialog (GTK_WINDOW (MainWindow),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK, _("This archive is encrypted!"),_("Please enter the password.") );
				break;
			}
			if (g_file_test (destination_path , G_FILE_TEST_EXISTS) == FALSE)
			{
				int result = mkdir (destination_path , S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXGRP);
				if (result == -1)
				{
					gchar *msg = g_strdup_printf(_("Can't create directory \"%s\""), destination_path);
					response = xa_show_message_dialog (GTK_WINDOW (MainWindow),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK, msg, g_strerror(errno ) );
					g_free (msg);
					break;
				}
			}
			if ( g_file_test (destination_path , G_FILE_TEST_IS_DIR) && access (destination_path, R_OK | W_OK | X_OK ) )
			{
				gchar *utf8_path;
				gchar  *msg;

                utf8_path = g_filename_to_utf8 (destination_path, -1, NULL, NULL, NULL);
                msg = g_strdup_printf (_("You don't have the right permissions to extract the files to the directory \"%s\"."), utf8_path);
				response = xa_show_message_dialog (GTK_WINDOW (MainWindow),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK, _("Can't perform extraction!"),msg );
                g_free (utf8_path);
				g_free (msg);
				g_free (destination_path);
				break;
			}
			done = TRUE;
			archive->overwrite = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON ( dialog_data->overwrite_check ));
			if ( dialog_data->touch != NULL)
				archive->tar_touch = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON ( dialog_data->touch ));

			if (dialog_data->extract_full != NULL)
				archive->full_path = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON ( dialog_data->extract_full ));

			if (dialog_data->fresh != NULL)
				archive->freshen = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON ( dialog_data->fresh ));

			if (dialog_data->update != NULL)
				archive->update = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON ( dialog_data->update ));

			//gtk_widget_set_sensitive (Stop_button,TRUE);
			gtk_widget_hide (dialog_data->dialog1);
			archive->status = XA_ARCHIVESTATUS_EXTRACT;
			/* Are all files selected? */
			if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON (dialog_data->all_files_radio )) )
			{
				if (MainWindow)
				{
					gchar *text = g_strdup_printf(_("Extracting files to %s"), destination_path);
					Update_StatusBar ( text );
					g_free (text);
				}
    		    tar = g_find_program_in_path ("gtar");
        		if (tar == NULL)
          			tar = g_strdup ("tar");
				switch ( archive->type )
				{
					case XARCHIVETYPE_BZIP2:
					lzma_gzip_bzip2_extract (archive);
					break;

					case XARCHIVETYPE_GZIP:
					lzma_gzip_bzip2_extract (archive);
					break;

					case XARCHIVETYPE_LZMA:
					lzma_gzip_bzip2_extract (archive);
					break;

					case XARCHIVETYPE_RAR:
					if (archive->passwd != NULL)
						command = g_strconcat ( rar, " " , archive->full_path ? "x " : "e ",
												archive->freshen ? "-f " : "" , archive->update ? "-u " : "",
												" -p",archive->passwd,
												archive->overwrite ? " -o+" : " -o-",
												" -idp ",
												archive->escaped_path , " " , archive->extraction_path , NULL );
					else
						command = g_strconcat ( rar, " ", archive->full_path ? "x " : "e ",
												archive->freshen ? "-f " : "" , archive->update ? "-u " : "",
												archive->overwrite ? "-o+" : "-o-",
												" -idp ",
												archive->escaped_path , " " , archive->extraction_path , NULL );
					break;

					case XARCHIVETYPE_TAR:
					if (archive->full_path == 1)
					{
						command = g_strconcat (tar, " -xvf ", archive->escaped_path,
											archive->overwrite ? " --overwrite" : " --keep-old-files",
											archive->tar_touch ? " --touch" : "",
											" -C " , archive->extraction_path , NULL );
					}
					else
					{
						xa_extract_tar_without_directories ( "tar -xvf ",archive,NULL,FALSE );
						command = NULL;
					}
					break;

					case XARCHIVETYPE_TAR_BZ2:
					if (archive->full_path == 1)
					{
						command = g_strconcat (tar, " -xvjf " , archive->escaped_path,
											archive->overwrite ? " --overwrite" : " --keep-old-files",
											archive->tar_touch ? " --touch" : "",
											" -C " , archive->extraction_path , NULL );
					}
					else
					{
						xa_extract_tar_without_directories ( "tar -xvjf ",archive,archive->extraction_path,FALSE );
						command = NULL;
					}
					break;

					case XARCHIVETYPE_TAR_GZ:
					if (archive->full_path == 1)
					{
						command = g_strconcat (tar, " -xvzf " , archive->escaped_path,
											archive->overwrite ? " --overwrite" : " --keep-old-files",
											archive->tar_touch ? " --touch" : "",
											" -C " , archive->extraction_path , NULL );
					}
					else
					{
						xa_extract_tar_without_directories ( "tar -xvzf ",archive,archive->extraction_path,FALSE );
						command = NULL;
					}
					break;

					case XARCHIVETYPE_TAR_LZMA:
					if (archive->full_path == 1)
					{
						command = g_strconcat (tar, " --use-compress-program=lzma -xvf " , archive->escaped_path,
											archive->overwrite ? " --overwrite" : " --keep-old-files",
											archive->tar_touch ? " --touch" : "",
											" -C " , archive->extraction_path , NULL );
					}
					else
					{
						xa_extract_tar_without_directories ( "tar --use-compress-program=lzma -xvf ",archive,NULL,FALSE);
						command = NULL;
					}
					break;

					case XARCHIVETYPE_DEB:
					if (archive->full_path == 1)
					{
						command = g_strconcat (tar, " -xvzf " , archive->tmp,
											archive->overwrite ? " --overwrite" : " --keep-old-files",
											archive->tar_touch ? " --touch" : "",
											" -C " , archive->extraction_path , NULL );
					}
					else
					{
						xa_extract_tar_without_directories ( "tar -xvzf ",archive,archive->extraction_path,FALSE );
						command = NULL;
					}
					break;

                    case XARCHIVETYPE_ZIP:
                    if ( archive->passwd != NULL )
						command = g_strconcat ( "unzip ", archive->freshen ? "-f " : "",
												archive->update ? "-u " : "" ,
												archive->overwrite ? "-o" : "-n",
												" -P " , archive->passwd,
												archive->full_path ? "" : " -j ",
												archive->escaped_path , " -d ", archive->extraction_path , NULL );
                    else
						command = g_strconcat ( "unzip ", archive->freshen ? "-f " : "",
												archive->update ? "-u " : "",
												archive->overwrite ? "-o " : "-n ",
												archive->full_path ? "" : " -j ",
												archive->escaped_path , " -d ", archive->extraction_path , NULL );
					break;

					case XARCHIVETYPE_RPM:
					if (archive->full_path == 1)
					{
						chdir ( archive->extraction_path );
						command = g_strconcat ( "cpio --make-directories -F " , archive->tmp , " -i" , NULL );
					}
					else
					{
						xa_extract_tar_without_directories ( "tar -xvzf " , archive,archive->escaped_path,TRUE);
						command = NULL;
					}
                    break;

                    case XARCHIVETYPE_7ZIP:
                    if (archive->passwd != NULL)
						command = g_strconcat ( "7za " , archive->full_path ? "x " : "e ",
												archive->overwrite ? "-aoa" : "-aos",
												" -bd -p",archive->passwd," ",
												archive->escaped_path , " -o" , archive->extraction_path , NULL );
					else
						command = g_strconcat ( "7za " , archive->full_path ? "x " : "e ",
												archive->overwrite ? "-aoa" : "-aos",
												" -bd ",
												archive->escaped_path , " -o" , archive->extraction_path , NULL );
                    break;

					case XARCHIVETYPE_ARJ:
					if (archive->passwd != NULL)
						command = g_strconcat ( "arj " , archive->full_path ? "x " : "e ",
												"-g",archive->passwd,
												archive->overwrite ? "" : " -n" , " -i ",
												archive->freshen ? "-f " : "" , archive->update ? "-u " : "",
												"-y " , archive->escaped_path , " " , archive->extraction_path , NULL );
                    else
						command = g_strconcat ( "arj " , archive->full_path ? "x " : "e ",
												archive->overwrite ? "" : " -n" , " -i ",
												archive->freshen ? "-f " : "" , archive->update ? "-u " : "",
												"-y " , archive->escaped_path , " " , archive->extraction_path , NULL );
					break;

					case XARCHIVETYPE_LHA:
					command = g_strconcat ("lha ", archive->full_path ? "x" : "xi",
											archive->overwrite ? "f" : "", "w=",
											archive->extraction_path, " ", archive->escaped_path , NULL);
					break;

					default:
					command = NULL;
				}
				g_free (tar);

				if (command != NULL)
					return command;
			}
			else
			{
				names = g_string_new ( " " );
				gtk_tree_selection_selected_foreach (selection, (GtkTreeSelectionForeachFunc) xa_concat_filenames, names );
				command = xa_extract_single_files ( archive , names, archive->extraction_path );
				g_string_free(names,TRUE);
			}
		}
	}
	g_free (destination_path);
	return command;
}

gchar *xa_extract_single_files (XArchive *archive,GString *files,gchar *path)
{
	gchar *command = NULL;
	gchar *tar;

	if (unrar)
		rar = "unrar";
	else
		rar = "rar";

	gchar *msg = g_strdup_printf ( _("Extracting archive to %s") , path);
	Update_StatusBar (msg);
	g_free (msg);
	tar = g_find_program_in_path ("gtar");
	if (tar == NULL)
		tar = g_strdup ("tar");
	switch (archive->type)
	{
		case XARCHIVETYPE_BZIP2:
		lzma_gzip_bzip2_extract (archive);
		break;

		case XARCHIVETYPE_GZIP:
		lzma_gzip_bzip2_extract (archive);
		break;

		case XARCHIVETYPE_LZMA:
		lzma_gzip_bzip2_extract (archive);
		break;

		case XARCHIVETYPE_RAR:
		if (archive->passwd != NULL)
			command = g_strconcat ( rar," ", archive->full_path ? "x " : "e " ,
									archive->freshen ? "-f " : "" , archive->update ? "-u " : "",
									" -p",archive->passwd,
									archive->overwrite ? " -o+" : " -o-",
									" -idp ",
									archive->escaped_path , " " , files->str , " " , path , NULL );
        else
			command = g_strconcat ( rar," ", archive->full_path ? "x " : "e " ,
									archive->freshen ? "-f " : "" , archive->update ? "-u " : "",
									archive->overwrite ? "-o+" : "-o-",
									" -idp ",
									archive->escaped_path , " " , files->str , " ", path ,NULL);
		break;

		case XARCHIVETYPE_TAR:
		if (archive->full_path == 1)
		{
			command = g_strconcat (tar, " -xvf " , archive->escaped_path,
								archive->overwrite ? " --overwrite" : " --keep-old-files",
								archive->tar_touch ? " --touch" : "",
								" -C " , path , files->str , NULL );
		}
		else
		{
			xa_extract_tar_without_directories ( "tar -xvf " , archive,path,FALSE );
			command = NULL;
		}
		break;

		case XARCHIVETYPE_TAR_BZ2:
		if (archive->full_path == 1)
		{
			command = g_strconcat (tar, " -xjvf " , archive->escaped_path,
								archive->overwrite ? " --overwrite" : " --keep-old-files",
								archive->tar_touch ? " --touch" : "",
								" -C " , path , files->str , NULL );
		}
		else
		{
			xa_extract_tar_without_directories ( "tar -xjvf " , archive,path,FALSE );
			command = NULL;
		}
		break;

		case XARCHIVETYPE_TAR_GZ:
		if (archive->full_path == 1)
		{
			command = g_strconcat (tar, " -xzvf " , archive->escaped_path,
								archive->overwrite ? " --overwrite" : " --keep-old-files",
								archive->tar_touch ? " --touch" : "",
								" -C " , path , files->str , NULL );
		}
		else
		{
			xa_extract_tar_without_directories ( "tar -xzvf " , archive,path,FALSE );
			command = NULL;
		}
		break;

		case XARCHIVETYPE_TAR_LZMA:
		if (archive->full_path == 1)
		{
			command = g_strconcat (tar, " --use-compress-program=lzma -xvf " , archive->escaped_path,
								archive->overwrite ? " --overwrite" : " --keep-old-files",
								archive->tar_touch ? " --touch" : "",
								" -C " , path , files->str , NULL );
		}
		else
		{
			xa_extract_tar_without_directories ( "tar --use-compress-program=lzma -xvf " , archive,path,FALSE );
			command = NULL;
		}
		break;

		case XARCHIVETYPE_DEB:
		if (archive->full_path == 1)
		{
			command = g_strconcat (tar, " -xvzf " , archive->tmp,
					archive->overwrite ? " --overwrite" : " --keep-old-files",
					archive->tar_touch ? " --touch" : "",
					" -C " , archive->extraction_path , files->str, NULL );
		}
		else
		{
			xa_extract_tar_without_directories ( "tar -xvzf " , archive,archive->tmp,FALSE );
			command = NULL;
		}
		break;

		case XARCHIVETYPE_ZIP:
        if ( archive->passwd != NULL )
			command = g_strconcat ( "unzip ", archive->freshen ? "-f " : "",
									archive->update ? "-u " : "",
									archive->overwrite ? "-o" : "-n",
									" -P " , archive->passwd,
									archive->full_path ? " " : " -j ",
									archive->escaped_path , files->str," -d " , path , NULL );
        else
			command = g_strconcat ( "unzip ", archive->freshen ? "-f " : "",
									archive->update ? "-u " : "",
									archive->overwrite ? "-o " : "-n ",
									archive->full_path ? "" : " -j ",
									archive->escaped_path , files->str," -d " , path , NULL );
		break;

        case XARCHIVETYPE_RPM:
        if (archive->full_path == 1)
		{
			chdir (path);
			command = g_strconcat ( "cpio --make-directories " , files->str , " -F " , archive->tmp , " -i" , NULL);
		}
		else
		{
			xa_extract_tar_without_directories ( "tar -xvzf " , archive,archive->escaped_path,TRUE);
			command = NULL;
		}
        break;

        case XARCHIVETYPE_7ZIP:
        if (archive->passwd != NULL)
			command = g_strconcat ("7za " , archive->full_path ? "x" : "e",
									" -p",archive->passwd,
									archive->overwrite ? " -aoa" : " -aos",
									" -bd ",
									archive->escaped_path , files->str , " -o" , path , NULL );
        else
			command = g_strconcat ( "7za " , archive->full_path ? "x" : "e",
									archive->overwrite ? " -aoa" : " -aos",
									" -bd ",
									archive->escaped_path , files->str , " -o" , path , NULL );
        break;

		case XARCHIVETYPE_ARJ:
		if (archive->passwd != NULL)
			command = g_strconcat ( "arj ",archive->full_path ? "x" : "e",
									" -g",archive->passwd,
									archive->overwrite ? "" : " -n" ,
									" -i " ,
									archive->freshen ? "-f " : "" ,
									archive->update ? "-u " : " ",
									"-y ",
									archive->escaped_path , " " , path , files->str , NULL );
        else
			command = g_strconcat ( "arj ",archive->full_path ? "x" : "e",
									archive->overwrite ? "" : " -n" ,
									" -i " , archive->freshen ? "-f " : "",
									archive->update ? "-u " : " ",
									"-y ",
									archive->escaped_path , " " , path , files->str, NULL );
		break;

		case XARCHIVETYPE_LHA:
			command = g_strconcat ("lha ", archive->full_path ? "x" : "xi",
										archive->overwrite ? "f" : "", "w=",
										path, " ", archive->escaped_path , files->str, NULL);
		break;

		default:
		command = NULL;
    }
	g_free (tar);
	return command;
}

gboolean xa_extract_tar_without_directories (gchar *string,XArchive *archive,gchar *extract_path,gboolean cpio_flag)
{
	gchar *command = NULL;
	gchar *name = NULL;
	gchar *_name = NULL;
	gchar *permission = NULL;
	gchar tmp_dir[14] = "";
	GtkTreeSelection *selection;
	GString *names;
	GtkTreeIter iter;
	GList *row_list;
	GSList *list = NULL;
	GSList *filenames = NULL;
	GSList *xxx = NULL;
	gboolean result;

	names = g_string_new ("");

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(archive->treeview));
	row_list = gtk_tree_selection_get_selected_rows(selection, &archive->model);

	if (row_list != NULL)
	{
		while (row_list)
		{
			gtk_tree_model_get_iter(archive->model, &iter,row_list->data);
			gtk_tree_model_get (archive->model, &iter,1, &name,3, &permission,-1);
			gtk_tree_path_free (row_list->data);

			if (strstr (permission ,"d") == NULL)
			{
				_name = g_strconcat (gtk_entry_get_text(GTK_ENTRY(location_entry)),name,NULL);
				g_free (name);
				name = _name;
				xa_shell_quote_filename (name,names,archive);
				xxx = g_slist_append (xxx,name);
			}
			g_free (permission);
			row_list = row_list->next;
		}
		g_list_free (row_list);
	}
	else
	{
		/* Here we need to fill a GSList with all the entries in the archive so to extract it entirely */
		XEntry *entry = archive->root_entry;
		while(entry)
		{
			xa_entries_to_filelist(entry, &xxx,"");
			entry = entry->next;
		}
		filenames = g_slist_reverse(xxx);
		/* Let's concatenate all the entries in one gstring */
		while (filenames)
		{
			g_string_prepend (names,(gchar*)filenames->data);
			g_string_prepend_c (names,' ');
			filenames = filenames->next;
		}
		xa_destroy_filelist (filenames);
		g_slist_free (filenames);
	}

	result = xa_create_temp_directory (archive,tmp_dir);
	if (result == 0)
		return FALSE;

	if (cpio_flag)
	{
		chdir (archive->tmp);
		command = g_strconcat ("cpio --make-directories -F ",archive->tmp," -i",NULL);
	}
	else
		command = g_strconcat (string, archive->escaped_path,
										archive->overwrite ? " --overwrite" : " --keep-old-files",
										archive->tar_touch ? " --touch" : "",
										" --no-wildcards -C ",
										archive->tmp,names->str,NULL);
	list = g_slist_append(list,command);

	if (extract_path == NULL)
		extract_path = archive->tmp;

	chdir (archive->tmp);
	command = g_strconcat ("mv -f ",names->str," ",extract_path,NULL);
	g_string_free(names,TRUE);

	list = g_slist_append(list,command);
	result = xa_run_command (archive,list);
	if (result == 0 || stop_flag)
		return FALSE;
	return result;
}

void xa_browse_dir(GtkTreeStore *model,gchar *path, GtkTreeIter *parent)
{
	DIR *dir;
	struct dirent *list;
	gchar *fullname;
	GtkTreeIter iter;
	GtkTreeIter dummy;

	dir = opendir(path);
	if (dir == NULL)
		return;
	while ((list = readdir(dir)))
	{
		if (list->d_name[0] == '.')
			continue;
		fullname = g_strconcat (path,"/",list->d_name,NULL);
		if (g_file_test(fullname,G_FILE_TEST_IS_DIR))
		{
			gtk_tree_store_append(model,&iter,parent);
			gtk_tree_store_set(model,&iter,0,"gtk-directory",1,list->d_name,2,fullname,-1);
			gtk_tree_store_append(model,&dummy,&iter);
		}
		g_free(fullname);
	}
	closedir(dir);
}

void xa_tree_view_row_selected(GtkTreeSelection *selection, gpointer data)
{
	GtkEntry *entry = data;
	gchar *dir;
	GtkTreeIter iter;
	GtkTreeModel *model;

	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
    	gtk_tree_model_get (model,&iter,2,&dir,-1);
    	gtk_entry_set_text(entry,dir);
    	g_free(dir);
    }
}

void xa_expand_dir(GtkTreeView *tree_view,GtkTreeIter *iter,GtkTreePath *path,gpointer data)
{
	GtkEntry *entry = data;
	GtkTreeModel *model;
	gchar *dir;
	GtkTreeIter child;
	gchar *fullname;

	model = gtk_tree_view_get_model(tree_view);
    gtk_tree_model_get(model,iter,2,&dir,-1);
	gtk_tree_model_iter_nth_child(model,&child,iter,0);
	gtk_tree_model_get(model,&child,1,&fullname,-1);
	if (fullname == NULL)
	{
		xa_browse_dir(GTK_TREE_STORE(model),dir,iter);
		gtk_tree_store_remove(GTK_TREE_STORE(model),&child);
	}
	else
		g_free(fullname);
	
	gtk_entry_set_text(entry,dir);
	g_free(dir);
}

void xa_choose_extraction_directory (GtkWidget *widget, gpointer data)
{
	Extract_dialog_data *dialog_data = data;
	GtkWidget *File_Selector;
	int response;
	gchar *path;

	File_Selector = gtk_file_chooser_dialog_new (_("Choose the destination directory"),
					GTK_WINDOW (MainWindow),
					GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
					GTK_STOCK_CANCEL,
					GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN,
					GTK_RESPONSE_ACCEPT,
					NULL);
	response = gtk_dialog_run (GTK_DIALOG (File_Selector));
	if (response == GTK_RESPONSE_ACCEPT)
	{
		path = gtk_file_chooser_get_filename ( GTK_FILE_CHOOSER (File_Selector) );
		gtk_entry_set_text (GTK_ENTRY(dialog_data->destination_path_entry),path);
		g_free (path);
	}
	gtk_widget_destroy (File_Selector);
}
