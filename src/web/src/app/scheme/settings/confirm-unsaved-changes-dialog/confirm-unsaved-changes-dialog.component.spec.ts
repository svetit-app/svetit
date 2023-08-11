import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ConfirmUnsavedChangesDialogComponent } from './confirm-unsaved-changes-dialog.component';

describe('ConfirmUnsavedChangesDialogComponent', () => {
  let component: ConfirmUnsavedChangesDialogComponent;
  let fixture: ComponentFixture<ConfirmUnsavedChangesDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ConfirmUnsavedChangesDialogComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ConfirmUnsavedChangesDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
